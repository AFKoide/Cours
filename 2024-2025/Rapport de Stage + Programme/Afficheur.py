#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import os
from datetime import datetime
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

COL_MODEL   = "model"
COL_CRIT    = "crit"
COL_LR      = "learning_rate"
COL_DELTA   = "delta"

# Priorité aux colonnes calc_* si elles existent
COLS = {
    "pref_recall":        ["calc_preference_recall", "preference_recall"],
    "pref_precision":     ["calc_preference_precision", "preference_precision"],
    "indiff_recall":      ["calc_indifference_recall", "indifference_recall"],
    "indiff_precision":   ["calc_indifference_precision", "indifference_precision"],
    "total_recall":       ["total_recall"],                # pas de calc_ fourni
    "total_precision":    ["calc_precision_total"],        # fourni
}

CRIT_ORDER = [
    ("linear",                "Linear"),
    ("game",                  "Game"),
    ("2-add",                 "Game 2-add"),
    ("MacSum",                "MacSum"),
    ("MacSum Imprécis,faible","MacSum Centered (weak)"),
    ("MacSum Imprécis,fort",  "MacSum Centered (strong)"),
]

def to_percent_if_needed(s: pd.Series) -> pd.Series:
    s2 = s.dropna()
    if len(s2) == 0:
        return s
    mn, mx = float(s2.min()), float(s2.max())
    # Si déjà en [0, 1], convertir en %
    if 0.0 <= mn and mx <= 1.0:
        return s * 100.0
    return s

def pick_first_existing(df: pd.DataFrame, candidates):
    for c in candidates:
        if c in df.columns:
            return c
    # Si aucune, on renvoie un nom improbable pour forcer vide
    return None

def aggregate_vs_delta(df: pd.DataFrame, y_col: str) -> pd.DataFrame:
    """Moyenne par delta (si plusieurs LR / runs)."""
    if y_col is None:
        return pd.DataFrame(columns=[COL_DELTA, "y"])
    tmp = df[[COL_DELTA, y_col]].copy()
    tmp = tmp.dropna(subset=[COL_DELTA, y_col])
    if tmp.empty:
        return pd.DataFrame(columns=[COL_DELTA, "y"])
    out = tmp.groupby(COL_DELTA, as_index=False)[y_col].mean()
    out = out.rename(columns={y_col: "y"})
    out = out.sort_values(COL_DELTA)
    return out

def plot_one_figure(df_model: pd.DataFrame, model_name: str, save=False, outdir="plots"):
    # Colonnes réelles choisies
    pref_recall_col      = pick_first_existing(df_model, COLS["pref_recall"])
    pref_precision_col   = pick_first_existing(df_model, COLS["pref_precision"])
    indiff_recall_col    = pick_first_existing(df_model, COLS["indiff_recall"])
    indiff_precision_col = pick_first_existing(df_model, COLS["indiff_precision"])
    total_recall_col     = pick_first_existing(df_model, COLS["total_recall"])
    total_precision_col  = pick_first_existing(df_model, COLS["total_precision"])

    # Conversion % si besoin
    for c in [pref_recall_col, pref_precision_col, indiff_recall_col, indiff_precision_col, total_recall_col, total_precision_col]:
        if c and c in df_model.columns:
            df_model[c] = to_percent_if_needed(df_model[c])

    fig, axes = plt.subplots(3, 2, figsize=(12, 12), constrained_layout=True)
    fig.suptitle(f"Comparaison par delta — {model_name}", fontsize=14)

    panels = [
        # (row, col, metric_col, title)
        (0, 0, indiff_recall_col,    "Indifference — Recall"),
        (0, 1, pref_recall_col,      "Preference — Recall"),
        (1, 0, indiff_precision_col, "Indifference — Precision"),
        (1, 1, pref_precision_col,   "Preference — Precision"),
        (2, 0, total_recall_col,     "Total Recall"),
        (2, 1, total_precision_col,  "Total Precision"),
    ]

    for (r, c, ycol, title) in panels:
        ax = axes[r, c]
        has_any = False
        for key, label in CRIT_ORDER:
            sub = df_model[df_model[COL_CRIT].astype(str).str.strip().str.lower() == key.lower()]
            if sub.empty or ycol is None or ycol not in sub.columns:
                continue
            curve = aggregate_vs_delta(sub, ycol)
            if curve.empty:
                continue
            # Tracé vs delta (log)
            ax.plot(curve[COL_DELTA].to_numpy(), curve["y"].to_numpy(), marker="o", label=label)
            has_any = True

        ax.set_title(title)
        ax.set_xlabel("delta")
        ax.set_xscale("log")
        ax.set_ylabel("%")
        ax.set_ylim(0, 100)
        ax.grid(True, alpha=0.3)
        if has_any:
            ax.legend(loc="best", fontsize=9)

    if save:
        os.makedirs(outdir, exist_ok=True)
        ts = datetime.now().strftime("%Y%m%d-%H%M%S")
        path = os.path.join(outdir, f"{model_name}_3x2_logdelta_{ts}.png")
        fig.savefig(path, dpi=150)
        print(f"[OK] Figure enregistrée : {path}")
        plt.close(fig)
    else:
        plt.show()

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--csv", required=True, help="Chemin du CSV")
    ap.add_argument("--save", action="store_true", help="Enregistrer les figures au lieu d'afficher")
    ap.add_argument("--outdir", default="plots", help="Dossier de sortie si --save")
    args = ap.parse_args()

    df = pd.read_csv(args.csv)
    df.columns = [c.strip() for c in df.columns]
    # Uniformiser
    df[COL_MODEL] = df[COL_MODEL].astype(str).str.strip()
    df[COL_CRIT]  = df[COL_CRIT].astype(str).str.strip()

    # delta numérique
    df[COL_DELTA] = pd.to_numeric(df[COL_DELTA], errors="coerce")

    for model_name in ["L1", "L2"]:
        sub = df[df[COL_MODEL].str.upper() == model_name.upper()].copy()
        if sub.empty:
            print(f"[Info] Aucune donnée pour model={model_name}")
            continue
        plot_one_figure(sub, model_name, save=args.save, outdir=args.outdir)

if __name__ == "__main__":
    main()
