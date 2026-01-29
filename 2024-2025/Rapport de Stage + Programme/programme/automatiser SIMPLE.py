import subprocess
import pandas as pd
import os
from pathlib import Path

CSV_OUTPUT_TRAIN    = "./releve_experimental_training.csv"
CSV_OUTPUT_TEST     = "./releve_experimental_test.csv"
PROG_EXECUTABLE     = "./prog.exe" if os.name == "nt" else "./prog"
BASE_PARAMS_PATH    = Path("./params.txt")     # Modèle de fichier params.txt












def lire_params(path=BASE_PARAMS_PATH):
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
        crit = "MacSum Imprécis"
    else:
        raise ValueError("Méthode non reconnue")

    # Manière (model)
    model = "L2" if maniere == 0 else "L1"

    if crit == "MacSum Imprécis":
        if ordre == 0:
            ordre = "FAIBLE"
        elif ordre == 1:
            ordre = "FORT"
    else:
        ordre = "INUTILISE"

    return model, crit, ordre, learning_rate, delta




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


def ajouter_ligne_csv(db, model, ordre, crit, learning_rate, delta, scores, sortie):
    ligne = {
        "model":                    model,
        "ordre":                    ordre,
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












def simulation_unique():
    # Lire configuration
    model, crit, ordre, learning_rate, delta = lire_params()

    # Lancer ./prog
    prog_name = "./prog.exe" if os.name == "nt" else "./prog"
    subprocess.run([prog_name], check=True)

    # Lire les CSV
    training_df = pd.read_csv("./training_log.csv")
    test_df = pd.read_csv("./test_log.csv")

    # Extraire les métriques
    scores_training = calculer_scores(training_df)
    scores_test = calculer_scores(test_df)

    # Ajouter au fichier de sortie
    ajouter_ligne_csv(training_df, model, ordre, crit, learning_rate, delta, scores_training, CSV_OUTPUT_TRAIN)
    ajouter_ligne_csv(test_df, model, ordre, crit, learning_rate, delta, scores_test, CSV_OUTPUT_TEST)







if __name__ == "__main__":
    N = 5  # nombre de simulations à lancer
    for i in range(N):
        print("===========================================")
        print("SIMULATION N°", i)
        print("\n")
        simulation_unique()

        print("\n\n\n")