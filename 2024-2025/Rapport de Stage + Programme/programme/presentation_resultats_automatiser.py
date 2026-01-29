import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def charger_donnees(fichier):
    df = pd.read_csv(fichier)
    return df

def regrouper_et_agreger(df):
    group = df.groupby(["model", "ordre", "crit"])
    tableau = []
    
    for (model, ordre, crit), grp in group:
        ligne = {
            "Crit.": crit,
            "Model": model,
            "Ordre": ordre,
            "Pref. recall": f"{grp['preference_recall'].mean():.3f} ± {grp['preference_recall'].std():.3f}",
            "Pref. precision": f"{grp['preference_precision'].mean():.3f} ± {grp['preference_precision'].std():.3f}",
            "Indif. recall": f"{grp['indifference_recall'].mean():.3f} ± {grp['indifference_recall'].std():.3f}",
            "Indif. precision": f"{grp['indifference_precision'].mean():.2e} ± {grp['indifference_precision'].std():.2e}",
            "Total recall": f"{grp['total_recall'].mean():.3f} ± {grp['total_recall'].std():.3f}"
        }
        tableau.append(ligne)

    return pd.DataFrame(tableau)

def generer_image_tableau(df, output="tableau_resultats.png"):
    fig, ax = plt.subplots(figsize=(12, len(df) * 0.7 + 1))
    ax.axis('off')
    tableau = ax.table(cellText=df.values, colLabels=df.columns, cellLoc='center', loc='center')
    tableau.auto_set_font_size(False)
    tableau.set_fontsize(10)
    tableau.scale(1.2, 1.5)
    plt.tight_layout()
    plt.savefig(output, dpi=300)
    print(f"✅ Image sauvegardée sous : {output}")

def main():
    df = charger_donnees("releve_experimental_training.csv")
    tableau = regrouper_et_agreger(df)
    tableau = tableau.sort_values(by=["Model", "Crit."])  # pour l'ordre comme sur ta photo
    generer_image_tableau(tableau, output="tableau_resultats_training.png")

    df = charger_donnees("releve_experimental_test.csv")
    tableau = regrouper_et_agreger(df)
    tableau = tableau.sort_values(by=["Model", "Crit."])  # pour l'ordre comme sur ta photo
    generer_image_tableau(tableau, output="tableau_resultats_test.png")

if __name__ == "__main__":
    main()