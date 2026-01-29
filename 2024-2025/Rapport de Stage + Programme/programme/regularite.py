#!/usr/bin/env python3
"""
batch_runner.py – Automatise l'exécution répétée de ./prog, renomme les CSV
produits et génère des graphiques de suivi.

Usage :
    # Génère 10 exécutions + figures
    python batch_runner.py -n 10

    # Ré-utilise les CSV déjà générés, ne relance pas ./prog
    python batch_runner.py --plot-only
"""

from __future__ import annotations
import argparse
import subprocess
import sys
from pathlib import Path
from scipy.signal import savgol_filter

import pandas as pd
import matplotlib
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import tkinter as tk
import matplotlib.pyplot as plt

CSV_BASENAMES = ("training_log.csv", "test_log.csv")

matplotlib.use("Agg")  # backend non interactif, ne crée pas de fenêtre

FILTRE = True



# --------------------------------------------------------------------------- #
# Utilities
# --------------------------------------------------------------------------- #
def run_once(run_idx: int, keep_old: bool) -> None:
    """Lance ./prog puis renomme les CSV générés en les suffixant _{run_idx}."""
    print(f"[RUN {run_idx}] Lancement de ./prog …")
    try:
        if sys.platform.startswith("win"):
            subprocess.run(["prog.exe"], check=True)
        else:
            subprocess.run(["./prog"], check=True)
    except (FileNotFoundError, subprocess.CalledProcessError) as e:
        sys.exit(f"❌ Échec de l’exécution de ./prog : {e}")

    for base in CSV_BASENAMES:
        src = Path(base)
        if not src.exists():
            sys.exit(f"❌ Fichier attendu non trouvé : {base}")
        dest = src.with_stem(f"{src.stem}_{run_idx}")
        if dest.exists() and not keep_old:
            dest.unlink()
        src.rename(dest)
        print(f"    → {dest.name} sauvegardé.")



def apply_savgol(signal, window_length=101, polyorder=3):
    if len(signal) < window_length:
        window_length = len(signal) if len(signal) % 2 == 1 else len(signal) - 1
    return savgol_filter(signal, window_length=window_length, polyorder=polyorder)

def gather_csv_logs(kind: str) -> list[Path]:
    """Retourne la liste triée des fichiers training_log_* ou test_log_* ."""
    pattern = f"{kind}_log_*.csv"
    return sorted(Path().glob(pattern), key=lambda p: int(p.stem.split("_")[-1]))


def plot_logs(kind: str, csv_paths: list[Path]) -> None:
    """Trace chaque colonne (sauf 'iteration' et 'erreur_moy') en pourcentage, avec scroll vertical si besoin."""
    if not csv_paths:
        print(f"Aucun fichier {kind}_log_* trouvé, graphique ignoré.")
        return


    dataframes = [pd.read_csv(p) for p in csv_paths]
    cols = [
        "erreur_moy",
        "nombre_egalite",
        "nombre_preference",
        "pred_vrai_egalite",
        "pred_fausse_egalite",
        "pred_vrai_preference",
        "pred_fausse_preference",
        "pred_fausse_inversion",
    ]
    LABELS = {
        "erreur_moy": "Erreur moyenne",
        "nombre_egalite": "Nb égalités",
        "nombre_preference": "Nb préférences",
        "pred_vrai_egalite": "X ~ X' vrai",
        "pred_fausse_egalite": "X ~ X' faux",
        "pred_vrai_preference": "X > X' vrai",
        "pred_fausse_preference": "X > X' faux",
        "pred_fausse_inversion": "X' > X",
    }
    
    
    # Création de la figure matplotlib
    fig, axes = plt.subplots(len(cols), 1, figsize=(9, 3 * len(cols)), sharex=True)
    if len(cols) == 1:
        axes = [axes]

    for df, path in zip(dataframes, csv_paths):
        label = path.stem
        total_egalite = df["nombre_egalite"]
        total_preference = df["nombre_preference"]

        for i, col in enumerate(cols):
            if col in ("pred_vrai_egalite", "pred_fausse_egalite"):
                pourcentage = 100 * df[col] / total_egalite.replace(0, 1)
            elif col in ("pred_vrai_preference", "pred_fausse_preference", "pred_fausse_inversion"):
                pourcentage = 100 * df[col] / total_preference.replace(0, 1)
            else:
                pourcentage = df[col]

            if FILTRE:
                pourcentage = apply_savgol(pourcentage)
            axes[i].plot(df["iteration"], pourcentage, label=label)
            axes[i].set_title(LABELS.get(col, col), fontsize=10, pad=10)
            axes[i].grid(True, alpha=0.3)

    axes[-1].set_xlabel("iteration")
    axes[0].set_title(f"{kind.capitalize()} logs – {len(csv_paths)} exécutions")
    axes[0].legend(fontsize=8, ncol=3, loc="upper center", bbox_to_anchor=(0.5, 1.25))
    if i > 2 : axes[i].set_ylim(-5, 105) 

    plt.tight_layout()

    # Ajout du scroll vertical avec tkinter
    root = tk.Tk()
    root.title(f"{kind.capitalize()} logs – {len(csv_paths)} exécutions")

    canvas = tk.Canvas(root)
    scrollbar = tk.Scrollbar(root, orient="vertical", command=canvas.yview)
    scrollable_frame = tk.Frame(canvas)

    scrollable_frame.bind(
        "<Configure>",
        lambda e: canvas.configure(
            scrollregion=canvas.bbox("all")
        )
    )

    canvas.create_window((0, 0), window=scrollable_frame, anchor="nw")
    canvas.configure(yscrollcommand=scrollbar.set)

    # Ajout du plot matplotlib dans le frame scrollable
    fig_canvas = FigureCanvasTkAgg(fig, master=scrollable_frame)
    fig_canvas.draw()
    fig_canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

    canvas.pack(side="left", fill="both", expand=True)
    scrollbar.pack(side="right", fill="y")

    root.mainloop()

# --------------------------------------------------------------------------- #
# 1) Un helper qui retourne simplement une Figure matplotlib
# --------------------------------------------------------------------------- #
def make_figure(kind: str, csv_paths: list[Path], save_mode=False) -> plt.Figure:
    if not csv_paths:
        print(f"Aucun fichier {kind}_log_* trouvé, graphique ignoré.")
        return None

    all_cols = [
        "erreur_moy",
        "nombre_egalite",
        "nombre_preference",
        "pred_vrai_egalite",
        "pred_fausse_egalite",
        "pred_vrai_preference",
        "pred_fausse_preference",
        "pred_fausse_inversion",
    ]

    # Exclure "nombre_preference" en mode --save
    cols = [c for c in all_cols if not (save_mode and c == "nombre_preference")]

    LABELS = {
        "erreur_moy": "Erreur moyenne",
        "nombre_egalite": "Nb égalités",
        "nombre_preference": "Nb préférences",
        "pred_vrai_egalite": "X ~ X' vrai",
        "pred_fausse_egalite": "X ~ X' faux",
        "pred_vrai_preference": "X > X' vrai",
        "pred_fausse_preference": "X > X' faux",
        "pred_fausse_inversion": "X' > X",
    }


    fig, axes = plt.subplots(len(cols), 1,
                             figsize=(9, 3 * len(cols)),
                             sharex=True)

    if len(cols) == 1:
        axes = [axes]

    for df, path in zip(map(pd.read_csv, csv_paths), csv_paths):
        label = path.stem
        total_eq = df["nombre_egalite"]
        total_pref = df["nombre_preference"]

        for i, col in enumerate(cols):
            if col in ("pred_vrai_egalite", "pred_fausse_egalite"):
                y = 100 * df[col] / total_eq.replace(0, 1)
            elif col in ("pred_vrai_preference",
                         "pred_fausse_preference",
                         "pred_fausse_inversion"):
                y = 100 * df[col] / total_pref.replace(0, 1)
            else:
                y = df[col]

            if FILTRE:
                y = apply_savgol(y)

            axes[i].plot(df["iteration"], y, label=label)
            axes[i].set_title(LABELS.get(col, col), fontsize=10, pad=10)
            axes[i].grid(True, alpha=0.3)
            if i > 2 : axes[i].set_ylim(-5, 105) 

    axes[-1].set_xlabel("iteration")
    axes[0].set_title(f"{kind.capitalize()} logs – {len(csv_paths)} exécutions")
    axes[0].legend(fontsize=8, ncol=3,
                   loc="upper center", bbox_to_anchor=(0.5, 1.25))
    fig.tight_layout()
    return fig


def save_combined_figure(training_data, test_data, nrows, filename="logs_summary.png"):
    fig, axes = plt.subplots(nrows, 2, figsize=(14, 3 * nrows), sharex=True)

    if nrows == 1:
        axes = [axes]  # handle single row case

    for i in range(nrows):
        # Colonne gauche = training
        for (x, y, title, label) in training_data[i::nrows]:
            axes[i][0].plot(x, y, label=label)
        axes[i][0].set_title(title, fontsize=10)
        axes[i][0].grid(True, alpha=0.3)

        # Colonne droite = test
        for (x, y, title, label) in test_data[i::nrows]:
            axes[i][1].plot(x, y, label=label)
        axes[i][1].set_title(title, fontsize=10)
        axes[i][1].grid(True, alpha=0.3)

    axes[-1][0].set_xlabel("iteration")
    axes[-1][1].set_xlabel("iteration")
    axes[0][0].legend(fontsize=8)
    fig.suptitle("Résumé des logs – Gauche: Training, Droite: Test", fontsize=14)
    fig.tight_layout(rect=[0, 0, 1, 0.97])
    fig.savefig(filename, dpi=300)
    print(f"✅ Image sauvegardée : {filename}")


def make_axes_data(kind: str, csv_paths: list[Path], save_mode=False):
    all_cols = [
        "erreur_moy",
        "nombre_egalite",
        "nombre_preference",
        "pred_vrai_egalite",
        "pred_fausse_egalite",
        "pred_vrai_preference",
        "pred_fausse_preference",
        "pred_fausse_inversion",
    ]
    LABELS = {
        "erreur_moy": "Erreur moyenne",
        "nombre_egalite": "Nb égalités",
        "nombre_preference": "Nb préférences",
        "pred_vrai_egalite": "X ~ X' vrai",
        "pred_fausse_egalite": "X ~ X' faux",
        "pred_vrai_preference": "X > X' vrai",
        "pred_fausse_preference": "X > X' faux",
        "pred_fausse_inversion": "X' > X",
    }
    cols = [c for c in all_cols if not (save_mode and c == "nombre_preference")]
    nrows = len(cols)
    axes_data = []

    for df, path in zip(map(pd.read_csv, csv_paths), csv_paths):
        label = path.stem
        total_eq = df["nombre_egalite"]
        total_pref = df["nombre_preference"]

        for col in cols:
            if col in ("pred_vrai_egalite", "pred_fausse_egalite"):
                y = 100 * df[col] / total_eq.replace(0, 1)
            elif col in ("pred_vrai_preference", "pred_fausse_preference", "pred_fausse_inversion"):
                y = 100 * df[col] / total_pref.replace(0, 1)
            else:
                y = df[col]

            if FILTRE:
                y = apply_savgol(y)

            axes_data.append((df["iteration"], y, LABELS.get(col, col), label))

    return axes_data, nrows




# --------------------------------------------------------------------------- #
# 2) Fenêtre unique avec canvas défilable
# --------------------------------------------------------------------------- #
def show_figures_side_by_side(figures: list[plt.Figure], titles: list[str]) -> None:
    root = tk.Tk()
    root.title("Training & Test logs")

    # Canvas avec scroll vertical
    canvas = tk.Canvas(root, highlightthickness=0)
    scrollbar = tk.Scrollbar(root, orient="vertical", command=canvas.yview)
    canvas.configure(yscrollcommand=scrollbar.set)

    scrollbar.pack(side="right", fill="y")
    canvas.pack(side="left", fill="both", expand=True)

    # Frame principal scrollable
    scrollable_frame = tk.Frame(canvas)
    canvas_window = canvas.create_window((0, 0), window=scrollable_frame, anchor="nw")

    # Adapter la largeur du frame à la taille de la fenêtre
    def on_configure(event):
        canvas.itemconfigure(canvas_window, width=event.width)
    canvas.bind("<Configure>", on_configure)

    # Mettre à jour le scrollregion quand le contenu change
    def update_scrollregion(event=None):
        canvas.configure(scrollregion=canvas.bbox("all"))
    scrollable_frame.bind("<Configure>", update_scrollregion)

    # Frame horizontal contenant deux colonnes
    columns_frame = tk.Frame(scrollable_frame)
    columns_frame.pack(fill="both", expand=True)

    for fig, title in zip(figures, titles):
        if fig is None:
            continue
        column = tk.Frame(columns_frame)
        column.pack(side="left", fill="both", expand=True, padx=10)

        tk.Label(column, text=title, font=("Helvetica", 12, "bold")).pack()
        fc = FigureCanvasTkAgg(fig, master=column)
        fc.draw()
        fc.get_tk_widget().pack(fill="both", expand=True)

    root.mainloop()




# --------------------------------------------------------------------------- #
# Script principal
# --------------------------------------------------------------------------- #
def main() -> None:
    parser = argparse.ArgumentParser(
        description="Répète l'exécution de ./prog et trace les CSV générés."
    )
    parser.add_argument(
        "-n",
        "--runs",
        type=int,
        default=0,
        help="Nombre d'exécutions de ./prog (0 = ne pas lancer).",
    )
    parser.add_argument(
        "--plot-only",
        action="store_true",
        help="Ignore ./prog et trace uniquement les CSV déjà présents.",
    )
    parser.add_argument(
        "--keep",
        action="store_true",
        help="Ne PAS supprimer les anciens CSV s'ils existent déjà.",
    )
    parser.add_argument(
        "--save",
        action="store_true",
        help="Sauvegarde les figures (sans affichage) en ignorant 'nombre_preference'."
    )
    args = parser.parse_args()

    # 1) Choix du mode : run + plot, ou plot-only
    if args.plot_only:
        if args.runs:
            print("⚠️  --plot-only ignore la valeur de --runs.")
        print("Mode 'plot-only' : aucune nouvelle exécution.")
    else:
        if args.runs <= 0:
            sys.exit("❌ Spécifie --runs N (>0) ou --plot-only.")
        for idx in range(1, args.runs + 1):
            run_once(idx, keep_old=args.keep)

    # 2) Lecture + tracé
    plt.ion()
    if args.save:
        training_data, nrows = make_axes_data("training", gather_csv_logs("training"), save_mode=True)
        test_data, _ = make_axes_data("test", gather_csv_logs("test"), save_mode=True)
        save_combined_figure(training_data, test_data, nrows)
    else:
        figures = [make_figure(kind, gather_csv_logs(kind)) for kind in ("training", "test")]
        show_figures_side_by_side(figures, titles=["Training logs", "Test logs"])
        plt.pause(0.1)
        input("Appuyez sur Entrée pour quitter…")
        plt.close("all")

if __name__ == "__main__":
    main()
