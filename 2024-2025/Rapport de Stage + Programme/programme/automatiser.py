import subprocess
import pandas as pd
import os
from sklearn.metrics import precision_score, recall_score, accuracy_score

def lire_params(path="./params.txt"):
    with open(path, "r") as f:
        lignes = [l.strip() for l in f if not l.startswith('%') and l.strip()]

    methode = int(lignes[6])
    maniere = int(lignes[7])
    k_add = int(lignes[8])

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
    model = "MSE" if maniere == 0 else "MAE"

    return model, crit

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
    preference_precision    = nb_pref_vrai   / (nb_pref_vrai + nb_pref_fausse + nb_indiff_fausse) if (nb_pref_vrai + nb_pref_fausse) > 0 else 0
    indifference_precision  = nb_indiff_vrai / (nb_indiff_vrai + nb_pref_inverse) if (nb_indiff_vrai + nb_pref_inverse) > 0 else 0

    # Accuracy: (TP + TN) / Total
    total = nb_indifferences + nb_preferences
    correct = nb_pref_vrai + nb_indiff_vrai
    precision_total = correct / total if total > 0 else 0

    return (preference_recall, preference_precision, indifference_recall, indifference_precision, precision_total)


def ajouter_ligne_csv(model, crit, scores, sortie):
    ligne = {
        "model": model,
        "crit": crit,
        "preference_recall": scores[0],
        "preference_precision": scores[1],
        "indifference_recall": scores[2],
        "indifference_precision": scores[3],
        "total_recall": scores[4],
    }

    df = pd.DataFrame([ligne])

    if not os.path.exists(sortie):
        df.to_csv(sortie, index=False)
    else:
        df.to_csv(sortie, mode='a', header=False, index=False)












def simulation_unique():
    # Lire configuration
    model, crit = lire_params()

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
    ajouter_ligne_csv(model, crit, scores_training, sortie="./releve_experimental_training.csv")
    ajouter_ligne_csv(model, crit, scores_test, sortie="./releve_experimental_test.csv")







if __name__ == "__main__":
    N = 5  # nombre de simulations à lancer
    for i in range(N):
        print("===========================================")
        print("SIMULATION N°", i)
        print("\n")
        simulation_unique()

        print("\n\n\n")