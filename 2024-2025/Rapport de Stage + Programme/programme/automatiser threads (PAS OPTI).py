import threading
from queue import Queue
import time
from pathlib import Path
import shutil
import subprocess
import pandas as pd
import os


# Verrou pour accès aux fichiers CSV partagés
csv_lock = threading.Lock()


# Configuration
N           = 20    # Nombre de simulations par combinaison
NUM_THREADS = 8     # Nombre de threads à utiliser

BASE_PARAMS_PATH    = Path("./base_params.txt")     # Modèle de fichier params.txt
WORKING_DIR         = Path("./working_dirs")        # Dossier de travail pour chaque thread
PROG_EXECUTABLE     = "./prog.exe" if os.name == "nt" else "./prog"
CSV_OUTPUT_TRAIN    = "./releve_experimental_training.csv"
CSV_OUTPUT_TEST     = "./releve_experimental_test.csv"


# Définition des combinaisons
learning_rates = [1e-1, 1e-2, 1e-5, 1e-8, 1e-9]
deltas = [1e-1, 1e-2, 1e-5, 1e-8, 1e-9, 1e-10, 1e-12, 1e-15]
methodes = [
    {"methode": 3, "k_add": -1, "ordre": 0},
    {"methode": 3, "k_add": -1, "ordre": 1},
]
manieres = [0, 1]



# ----------------------------------
def lire_params(path):
    with open(path, "r") as f:
        lignes = [l.strip() for l in f if not l.startswith('%') and l.strip()]

    methode = int(lignes[6])
    maniere = int(lignes[7])
    k_add = int(lignes[8])
    ordre = int(lignes[9])
    learning_rate = float(lignes[10])
    delta = float(lignes[11])

    # Méthode (crit)
    if methode == 0:
        crit = "linear"
    elif methode == 1:
        crit = "game" if k_add == -1 else "2-add"
    elif methode == 2:
        crit = "MacSum"
    elif methode == 3:
        base = "MacSum Imprécis"
        ordre_str = "faible" if ordre == 0 else "fort"
        crit = f"{base},{ordre_str}"
    else:
        raise ValueError("Méthode non reconnue")

    model = "L2" if maniere == 0 else "L1"

    return model, crit



def ecrire_params(methode, maniere, k_add, ordre, learning_rate, delta, path):
    with open(path, "r") as f:
        lignes = f.readlines()

    # On cherche les indices des lignes effectives (celles non commentées)
    lignes_effectives = [i for i, l in enumerate(lignes) if not l.strip().startswith('%') and l.strip()]

    # On suppose que les lignes 6 à 11 sont bien aux bons indices
    try:
        lignes[lignes_effectives[6]]  = f"{methode}\n"
        lignes[lignes_effectives[7]]  = f"{maniere}\n"
        lignes[lignes_effectives[8]]  = f"{k_add}\n"
        lignes[lignes_effectives[9]]  = f"{ordre}\n"
        lignes[lignes_effectives[10]] = f"{learning_rate:.0e}\n"
        lignes[lignes_effectives[11]] = f"{delta:.0e}\n"
    except IndexError:
        raise RuntimeError("Le fichier params.txt n'a pas le bon format ou nombre de lignes effectives.")

    with open(path, "w") as f:
        f.writelines(lignes)




# ----------------------------------
def ajouter_ligne_csv(db, model, crit, learning_rate, delta, scores, sortie):
    ligne = {
        "model":                    model,
        "crit":                     crit,
        "learning_rate":            learning_rate,
        "delta":                    delta,
        "preference_recall":        scores[0],
        "preference_precision":     scores[1],
        "indifference_recall":      scores[2],
        "indifference_precision":   scores[3],
        "total_recall":             scores[4],
        "nb_indifferences":         db["nombre_egalite"].iloc[-1],
        "nb_preferences":           db["nombre_preference"].iloc[-1],
        "nb_indiff_vrai":           db["pred_vrai_egalite"].iloc[-1],
        "nb_indiff_fausse":         db["pred_fausse_egalite"].iloc[-1],
        "nb_pref_vrai":             db["pred_vrai_preference"].iloc[-1],
        "nb_pref_fausse":           db["pred_fausse_preference"].iloc[-1],
        "nb_pref_inverse":          db["pred_fausse_inversion"].iloc[-1]
    }

    df = pd.DataFrame([ligne])

    if not os.path.exists(sortie):
        df.to_csv(sortie, index=False)
    else:
        df.to_csv(sortie, mode='a', header=False, index=False)




# ----------------------------------
def calculer_scores(db):
    nb_indifferences = db["nombre_egalite"].iloc[-1]
    nb_preferences   = db["nombre_preference"].iloc[-1]
    nb_indiff_vrai   = db["pred_vrai_egalite"].iloc[-1]
    nb_indiff_fausse = db["pred_fausse_egalite"].iloc[-1]
    nb_pref_vrai     = db["pred_vrai_preference"].iloc[-1]
    nb_pref_fausse   = db["pred_fausse_preference"].iloc[-1]
    nb_pref_inverse  = db["pred_fausse_inversion"].iloc[-1]

    # Recall: TP / (TP + FN)
    preference_recall   = nb_pref_vrai   / (nb_pref_vrai + nb_pref_fausse + nb_pref_inverse) if (nb_pref_vrai + nb_pref_fausse + nb_pref_inverse) > 0 else 0
    indifference_recall = nb_indiff_vrai / (nb_indiff_vrai + nb_indiff_fausse) if (nb_indiff_vrai + nb_indiff_fausse) > 0 else 0

    # Precision: TP / (TP + FP)
    preference_precision    = nb_pref_vrai   / (nb_pref_vrai + nb_pref_fausse) if (nb_pref_vrai + nb_pref_fausse) > 0 else 0
    indifference_precision  = nb_indiff_vrai / (nb_indiff_vrai + nb_pref_inverse) if (nb_indiff_vrai + nb_pref_inverse) > 0 else 0

    # Accuracy: (TP + TN) / Total
    total = nb_indifferences + nb_preferences
    correct = nb_pref_vrai + nb_indiff_vrai
    precision_total = correct / total if total > 0 else 0

    return (preference_recall, preference_precision, indifference_recall, indifference_precision, precision_total)




# ----------------------------------
def worker(thread_id, task_queue):
    working_dir = WORKING_DIR / f"thread_{thread_id}"
    working_dir.mkdir(parents=True, exist_ok=True)
    params_path = working_dir / "params.txt"

    shutil.copy(BASE_PARAMS_PATH, params_path)

    while True:
        try:
            idx, (methode, maniere, learning_rate, delta) = task_queue.get_nowait()
        except:
            # plus de tâches
            update_progress(thread_id, status="done", combo_idx=None, combo_str="", iter=0)
            break

        combo_str = combo_to_str(idx, methode, maniere, learning_rate, delta)
        update_progress(thread_id,
                        status="running",
                        combo_idx=idx,
                        combo_str=combo_str,
                        iter=0)

        # Mise à jour params
        ecrire_params(
            methode=methode["methode"],
            maniere=maniere,
            k_add=methode["k_add"],
            ordre=methode["ordre"],
            learning_rate=learning_rate,
            delta=delta,
            path=params_path
        )

        model, crit = lire_params(params_path)

        for i in range(N):
            # >>> maj itération
            update_progress(thread_id, iter=i)

            result = subprocess.run(
                [PROG_EXECUTABLE, str(working_dir)],
                capture_output=True,
                text=True
            )

            if result.returncode != 0:
                print(f"[Thread {thread_id}] Erreur exécution idx={idx} i={i} : {result.stderr}")
                update_progress(thread_id, errors=progress_state[thread_id]["errors"] + 1)
                continue

            training_csv = working_dir / "training_log.csv"
            test_csv     = working_dir / "test_log.csv"

            try:
                train_df = pd.read_csv(training_csv)
                test_df  = pd.read_csv(test_csv)

                # Skip si vide
                if train_df.empty or test_df.empty:
                    print(f"[Thread {thread_id}] CSV vide idx={idx} i={i}")
                    update_progress(thread_id, errors=progress_state[thread_id]["errors"] + 1)
                    continue

                train_scores = calculer_scores(train_df)
                test_scores  = calculer_scores(test_df)

                with csv_lock:
                    ajouter_ligne_csv(train_df, model, crit, learning_rate, delta, train_scores, CSV_OUTPUT_TRAIN)
                    ajouter_ligne_csv(test_df,  model, crit, learning_rate, delta, test_scores, CSV_OUTPUT_TEST)
            except Exception as e:
                print(f"[Thread {thread_id}] Erreur lecture CSV idx={idx} i={i} : {e}")
                update_progress(thread_id, errors=progress_state[thread_id]["errors"] + 1)
                continue

        print(f"[Thread {thread_id}] Combinaison {idx} terminée")
        # >>> combo terminé : repasser en idle
        update_progress(thread_id, status="idle", combo_idx=None, combo_str="", iter=0)



# --- Progression dynamique ---
progress_lock = threading.Lock()
progress_state = {}
stop_display_event = threading.Event()

def combo_to_str(idx, m, maniere, lr, delta):
    return (f"#{idx} m={m['methode']} k={m['k_add']} o={m['ordre']} "
            f"man={maniere} lr={lr:.0e} d={delta:.0e}")

def init_progress(num_threads, iter_total):
    with progress_lock:
        for tid in range(num_threads):
            progress_state[tid] = {
                "status": "idle",
                "combo_idx": None,
                "combo_str": "",
                "iter": 0,
                "iter_total": iter_total,
                "errors": 0,
            }

def update_progress(thread_id, **kwargs):
    with progress_lock:
        st = progress_state[thread_id]
        st.update(kwargs)

def display_loop():
    while not stop_display_event.is_set():
        with progress_lock:
            lines = []
            for tid in sorted(progress_state):
                st = progress_state[tid]
                lines.append(
                    f"Thread {tid:2d} | {st['status']:7s} | "
                    f"{st['iter']:02d}/{st['iter_total']:02d} | "
                    f"{st['combo_str']} | err={st['errors']}"
                )
        print("\033[2J\033[H", end="")  # effacer écran + curseur en haut
        print("== Progression des simulations ==")
        for L in lines:
            print(L)
        print("(Ctrl-C pour interrompre)")
        time.sleep(5)





















if __name__ == "__main__":
    WORKING_DIR.mkdir(parents=True, exist_ok=True)

    if not BASE_PARAMS_PATH.exists():
        raise FileNotFoundError(f"Le fichier de base des paramètres '{BASE_PARAMS_PATH}' n'existe pas.")

    combinations = [
        (methode, maniere, learning_rate, delta)
        for learning_rate in learning_rates
        for delta in deltas
        for methode in methodes
        for maniere in manieres
    ]

    task_queue = Queue()
    for idx, combo in enumerate(combinations):
        task_queue.put((idx, combo))

    # Initialiser la table de progression
    init_progress(NUM_THREADS, N)

    # Thread d'affichage
    disp_thread = threading.Thread(target=display_loop, daemon=True)
    disp_thread.start()

    # Threads de travail
    threads = []
    for i in range(NUM_THREADS):
        t = threading.Thread(target=worker, args=(i, task_queue))
        t.start()
        threads.append(t)

    for t in threads:
        t.join()

    # Arrêter l'affichage
    stop_display_event.set()
    disp_thread.join(timeout=1)

    # Dernier affichage figé
    with progress_lock:
        print("\n== État final ==")
        for tid, st in progress_state.items():
            print(f"Thread {tid:2d} | {st['status']:7s} | {st['iter']:02d}/{st['iter_total']:02d} | {st['combo_str']} | err={st['errors']}")

    print("✅ Toutes les simulations sont terminées.")

