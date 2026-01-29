from PyQt5.QtWidgets import QApplication, QTabWidget, QWidget, QVBoxLayout, QMainWindow, QPushButton, QHBoxLayout
import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import savgol_filter
from sklearn.metrics import confusion_matrix, ConfusionMatrixDisplay, classification_report
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
import sys

LINEAR = 0
GAME = 1
MACSUM = 2
MACSUM_SANS_TOL = 3

RAPPORT = False

test_log_path = r"test_log.csv"
training_log_path = r"training_log.csv"

class PlotTabs(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Visualisation des logs")
        self.tabs = QTabWidget()
        self.main_widget = QWidget()
        self.main_layout = QVBoxLayout()
        self.button_layout = QHBoxLayout()
        self.refresh_button = QPushButton("Refresh")
        self.refresh_button.clicked.connect(self.refresh)
        self.button_layout.addWidget(self.refresh_button)
        self.main_layout.addLayout(self.button_layout)
        self.main_layout.addWidget(self.tabs)
        self.main_widget.setLayout(self.main_layout)
        self.setCentralWidget(self.main_widget)
        self.init_tabs()

    @staticmethod
    def apply_savgol(signal, window_length=101, polyorder=3):
        if len(signal) == 0:
            return np.array(signal)
        if len(signal) < window_length:
            window_length = len(signal) if len(signal) % 2 == 1 else max(1, len(signal) - 1)
            if window_length < 3:  # trop court pour savgol
                return np.array(signal)
        return savgol_filter(signal, window_length=window_length, polyorder=polyorder)

    def refresh(self):
        self.tabs.clear()
        self.init_tabs()

    def load_logs(self):
        global test_log, training_log
        test_log = pd.read_csv(test_log_path)
        training_log = pd.read_csv(training_log_path)
        training_log.columns = training_log.columns.str.strip()
        test_log.columns = test_log.columns.str.strip()

        # Store raw series (train)
        self.train_iteration            = training_log["iteration"].to_numpy()
        self.train_erreur_moyenne       = training_log["erreur_moy"].to_numpy()
        self.train_nombre_egalite       = training_log["nombre_egalite"].to_numpy()
        self.train_nombre_preference    = training_log["nombre_preference"].to_numpy()
        self.train_pred_vrai_egalite    = training_log["pred_vrai_egalite"].to_numpy()
        self.train_pred_fausse_egalite  = training_log["pred_fausse_egalite"].to_numpy()
        self.train_pred_vrai_preference = training_log["pred_vrai_preference"].to_numpy()
        self.train_pred_fausse_preference = training_log["pred_fausse_preference"].to_numpy()
        self.train_pred_fausse_inversion  = training_log["pred_fausse_inversion"].to_numpy()

        # Store raw series (test)
        self.test_iteration            = test_log["iteration"].to_numpy()
        self.test_erreur_moyenne       = test_log["erreur_moy"].to_numpy()
        self.test_nombre_egalite       = test_log["nombre_egalite"].to_numpy()
        self.test_nombre_preference    = test_log["nombre_preference"].to_numpy()
        self.test_pred_vrai_egalite    = test_log["pred_vrai_egalite"].to_numpy()
        self.test_pred_fausse_egalite  = test_log["pred_fausse_egalite"].to_numpy()
        self.test_pred_vrai_preference = test_log["pred_vrai_preference"].to_numpy()
        self.test_pred_fausse_preference = test_log["pred_fausse_preference"].to_numpy()
        self.test_pred_fausse_inversion  = test_log["pred_fausse_inversion"].to_numpy()

        # Compute scores over time
        self.train_scores = self.compute_scores(
            self.train_pred_vrai_preference,
            self.train_pred_fausse_preference,
            self.train_pred_fausse_inversion,
            self.train_pred_vrai_egalite,
            self.train_pred_fausse_egalite,
            self.train_nombre_preference,
            self.train_nombre_egalite
        )
        self.test_scores = self.compute_scores(
            self.test_pred_vrai_preference,
            self.test_pred_fausse_preference,
            self.test_pred_fausse_inversion,
            self.test_pred_vrai_egalite,
            self.test_pred_fausse_egalite,
            self.test_nombre_preference,
            self.test_nombre_egalite
        )

    @staticmethod
    def safe_div(num, den):
        den = np.where(den == 0, np.nan, den)
        out = num / den
        return np.nan_to_num(out, nan=0.0, posinf=0.0, neginf=0.0)

    def compute_scores(self,
                       nb_pref_vrai, nb_pref_fausse, nb_pref_inverse,
                       nb_indiff_vrai, nb_indiff_fausse,
                       nb_preferences, nb_indifferences):
        """
        Calcule : recall/precision pour 'Préférence' et 'Indifférence',
        + accuracy globale par itération.
        Rappels (par itération):
          preference_recall   = TP_pref / (TP_pref + FN_pref) = nb_pref_vrai / (nb_pref_vrai + nb_pref_fausse + nb_pref_inverse)
          indifference_recall = TP_ind  / (TP_ind  + FN_ind ) = nb_indiff_vrai / (nb_indiff_vrai + nb_indiff_fausse)

          preference_precision  = TP_pref / (TP_pref + FP_pref) = nb_pref_vrai / (nb_pref_vrai + nb_pref_fausse)
          indifference_precision= TP_ind  / (TP_ind  + FP_ind ) = nb_indiff_vrai / (nb_indiff_vrai + nb_pref_inverse)

          accuracy = (TP_pref + TP_ind) / (nb_preferences + nb_indifferences)
        """
        # Recall
        preference_recall = self.safe_div(nb_pref_vrai, nb_pref_vrai + nb_pref_fausse + nb_pref_inverse)
        indiff_recall     = self.safe_div(nb_indiff_vrai, nb_indiff_vrai + nb_indiff_fausse)

        # Precision
        preference_precision = self.safe_div(nb_pref_vrai, nb_pref_vrai + nb_pref_fausse + nb_pref_inverse)
        indiff_precision     = self.safe_div(nb_indiff_vrai, nb_indiff_vrai + nb_pref_inverse)

        # Accuracy
        total   = nb_preferences + nb_indifferences
        correct = nb_pref_vrai + nb_indiff_vrai
        accuracy = self.safe_div(correct, total)

        # Pack + versions lissées
        def sg(x): return self.apply_savgol(x)

        return {
            "preference_recall": preference_recall,
            "indiff_recall": indiff_recall,
            "preference_precision": preference_precision,
            "indiff_precision": indiff_precision,
            "accuracy": accuracy,

            "preference_recall_s": sg(preference_recall),
            "indiff_recall_s": sg(indiff_recall),
            "preference_precision_s": sg(preference_precision),
            "indiff_precision_s": sg(indiff_precision),
            "accuracy_s": sg(accuracy),
        }

    def init_tabs(self):
        self.load_logs()

        # ===== Tab 1: Courbes existantes =====
        tab1 = QWidget()
        layout1 = QVBoxLayout()
        fig1 = Figure(figsize=(14, 6))
        canvas1 = FigureCanvas(fig1)
        layout1.addWidget(canvas1)
        tab1.setLayout(layout1)
        self.tabs.addTab(tab1, "Courbes")

        ax1 = fig1.add_subplot(3, 2, 1)
        ax1.plot(self.train_iteration, self.train_erreur_moyenne, label="Erreur Moyenne", color='blue')
        ax1.set_title("Logs de Training")
        ax1.set_xlabel("Itération")
        ax1.set_ylabel("Erreur Moyenne")
        ax1.legend(); ax1.grid()

        ax2 = fig1.add_subplot(3, 2, 2)
        ax2.plot(self.test_iteration, self.test_erreur_moyenne, label="Erreur Moyenne", color='green')
        ax2.set_title("Logs de Test")
        ax2.set_xlabel("Itération")
        ax2.set_ylabel("Erreur Moyenne")
        ax2.legend(); ax2.grid()

        ax3 = fig1.add_subplot(3, 2, 3)
        train_vraies_egalites = self.safe_div(self.train_pred_vrai_egalite, self.train_nombre_egalite) * 100
        train_fausses_egalites = self.safe_div(self.train_pred_fausse_egalite, self.train_nombre_egalite) * 100
        ax3.plot(self.train_iteration[:len(train_vraies_egalites)], self.apply_savgol(train_vraies_egalites), label="Vraies égalités (%)")
        ax3.plot(self.train_iteration[:len(train_fausses_egalites)], self.apply_savgol(train_fausses_egalites), label="Fausses égalités (%)", linestyle="--")
        ax3.set_xlabel("Itération"); ax3.set_ylabel("Pourcentage"); ax3.legend(); ax3.grid()

        ax4 = fig1.add_subplot(3, 2, 5)
        train_vraies_pref = self.safe_div(self.train_pred_vrai_preference, self.train_nombre_preference) * 100
        train_fausses_pref = self.safe_div(self.train_pred_fausse_preference, self.train_nombre_preference) * 100
        train_fausses_inv  = self.safe_div(self.train_pred_fausse_inversion,  self.train_nombre_preference) * 100
        ax4.plot(self.train_iteration, self.apply_savgol(train_vraies_pref), label="Vraies préférences (%)")
        ax4.plot(self.train_iteration, self.apply_savgol(train_fausses_pref), label="Fausses préférences (%)", linestyle="--")
        ax4.plot(self.train_iteration, self.apply_savgol(train_fausses_inv), label="Fausses inversions (%)", linestyle="--")
        ax4.set_xlabel("Itération"); ax4.set_ylabel("Pourcentage"); ax4.legend(); ax4.grid()

        ax5 = fig1.add_subplot(3, 2, 4)
        test_vraies_egalites = self.safe_div(self.test_pred_vrai_egalite, self.test_nombre_egalite) * 100
        test_fausses_egalites = self.safe_div(self.test_pred_fausse_egalite, self.test_nombre_egalite) * 100
        ax5.plot(self.test_iteration[:len(test_vraies_egalites)], self.apply_savgol(test_vraies_egalites), label="Vraies égalités (%)")
        ax5.plot(self.test_iteration[:len(test_fausses_egalites)], self.apply_savgol(test_fausses_egalites), label="Fausses égalités (%)", linestyle="--")
        ax5.set_xlabel("Itération"); ax5.set_ylabel("Pourcentage"); ax5.legend(); ax5.grid()

        ax6 = fig1.add_subplot(3, 2, 6)
        test_vraies_pref = self.safe_div(self.test_pred_vrai_preference, self.test_nombre_preference) * 100
        test_fausses_pref = self.safe_div(self.test_pred_fausse_preference, self.test_nombre_preference) * 100
        test_fausses_inv  = self.safe_div(self.test_pred_fausse_inversion,  self.test_nombre_preference) * 100
        ax6.plot(self.test_iteration, self.apply_savgol(test_vraies_pref), label="Vraies préférences (%)")
        ax6.plot(self.test_iteration, self.apply_savgol(test_fausses_pref), label="Fausses préférences (%)", linestyle="--")
        ax6.plot(self.test_iteration, self.apply_savgol(test_fausses_inv), label="Fausses inversions (%)", linestyle="--")
        ax6.set_xlabel("Itération"); ax6.set_ylabel("Pourcentage"); ax6.legend(); ax6.grid()

        fig1.tight_layout()

        # ===== Tab 2: Matrices & Rapports (inchangé) =====
        tab2 = QWidget()
        layout2 = QVBoxLayout()
        fig2 = Figure(figsize=(14, 8))
        canvas2 = FigureCanvas(fig2)
        layout2.addWidget(canvas2)
        tab2.setLayout(layout2)
        self.tabs.addTab(tab2, "Matrices & Rapports")

        axes = fig2.subplots(2, 2)
        last_idx = -1

        y_true_train = np.concatenate([
            np.repeat(0, self.train_nombre_egalite[last_idx]),
            np.repeat(1, self.train_nombre_preference[last_idx])
        ])
        y_pred_train = np.concatenate([
            np.repeat(0, self.train_pred_vrai_egalite[last_idx]),
            np.repeat(1, self.train_pred_fausse_egalite[last_idx]),
            np.repeat(1, self.train_pred_vrai_preference[last_idx]),
            np.repeat(0, self.train_pred_fausse_preference[last_idx]),
            np.repeat(1, self.train_pred_fausse_inversion[last_idx])
        ])[:len(y_true_train)]
        cm_train = confusion_matrix(y_true_train, y_pred_train, labels=[0, 1])
        disp_train = ConfusionMatrixDisplay(confusion_matrix=cm_train, display_labels=["Egalité", "Préférence"])
        disp_train.plot(ax=axes[0, 0], cmap=plt.cm.Blues, colorbar=False)
        axes[0, 0].set_title("Matrice de confusion apprentissage")

        y_true_test = np.concatenate([
            np.repeat(0, self.test_nombre_egalite[last_idx]),
            np.repeat(1, self.test_nombre_preference[last_idx])
        ])
        y_pred_test = np.concatenate([
            np.repeat(0, self.test_pred_vrai_egalite[last_idx]),
            np.repeat(1, self.test_pred_fausse_egalite[last_idx]),
            np.repeat(1, self.test_pred_vrai_preference[last_idx]),
            np.repeat(0, self.test_pred_fausse_preference[last_idx]),
            np.repeat(1, self.test_pred_fausse_inversion[last_idx])
        ])[:len(y_true_test)]
        cm_test = confusion_matrix(y_true_test, y_pred_test, labels=[0, 1])
        disp_test = ConfusionMatrixDisplay(confusion_matrix=cm_test, display_labels=["Egalité", "Préférence"])
        disp_test.plot(ax=axes[0, 1], cmap=plt.cm.Blues, colorbar=False)
        axes[0, 1].set_title("Matrice de confusion test")

        axes[1, 0].axis('off')
        axes[1, 0].text(0.5, 0.5, f"Rapport de classification (apprentissage):\n\n{classification_report(y_true_train, y_pred_train, target_names=['Egalité', 'Préférence'], digits=4)}",
                        fontsize=10, va='center', ha='center', family='monospace', wrap=True)
        axes[1, 1].axis('off')
        axes[1, 1].text(0.5, 0.5, f"Rapport de classification (test):\n\n{classification_report(y_true_test, y_pred_test, target_names=['Egalité', 'Préférence'], digits=4)}",
                        fontsize=10, va='center', ha='center', family='monospace', wrap=True)
        fig2.tight_layout()

        # ===== Tab 3: Scores au cours du temps =====
        tab3 = QWidget()
        layout3 = QVBoxLayout()
        fig3 = Figure(figsize=(14, 8))
        canvas3 = FigureCanvas(fig3)
        layout3.addWidget(canvas3)
        tab3.setLayout(layout3)
        self.tabs.addTab(tab3, "Scores")

        ax7 = fig3.add_subplot(3, 1, 1)  # Recall
        ax7.plot(self.train_iteration, self.train_scores["preference_recall"], alpha=0.25, label="Train Pref Recall (raw)")
        ax7.plot(self.train_iteration, self.train_scores["preference_recall_s"], label="Train Pref Recall (smooth)")
        ax7.plot(self.train_iteration, self.train_scores["indiff_recall"], alpha=0.25, label="Train Indiff Recall (raw)")
        ax7.plot(self.train_iteration, self.train_scores["indiff_recall_s"], label="Train Indiff Recall (smooth)")
        ax7.plot(self.test_iteration,  self.test_scores["preference_recall_s"], linestyle="--", label="Test Pref Recall (smooth)")
        ax7.plot(self.test_iteration,  self.test_scores["indiff_recall_s"], linestyle="--", label="Test Indiff Recall (smooth)")
        ax7.set_title("Recall au cours du temps"); ax7.set_xlabel("Itération"); ax7.set_ylabel("Recall")
        ax7.legend(); ax7.grid()

        ax8 = fig3.add_subplot(3, 1, 2)  # Precision
        ax8.plot(self.train_iteration, self.train_scores["preference_precision"], alpha=0.25, label="Train Pref Precision (raw)")
        ax8.plot(self.train_iteration, self.train_scores["preference_precision_s"], label="Train Pref Precision (smooth)")
        ax8.plot(self.train_iteration, self.train_scores["indiff_precision"], alpha=0.25, label="Train Indiff Precision (raw)")
        ax8.plot(self.train_iteration, self.train_scores["indiff_precision_s"], label="Train Indiff Precision (smooth)")
        ax8.plot(self.test_iteration,  self.test_scores["preference_precision_s"], linestyle="--", label="Test Pref Precision (smooth)")
        ax8.plot(self.test_iteration,  self.test_scores["indiff_precision_s"], linestyle="--", label="Test Indiff Precision (smooth)")
        ax8.set_title("Precision au cours du temps"); ax8.set_xlabel("Itération"); ax8.set_ylabel("Precision")
        ax8.legend(); ax8.grid()

        ax9 = fig3.add_subplot(3, 1, 3)  # Accuracy globale
        ax9.plot(self.train_iteration, self.train_scores["accuracy"], alpha=0.25, label="Train Accuracy (raw)")
        ax9.plot(self.train_iteration, self.train_scores["accuracy_s"], label="Train Accuracy (smooth)")
        ax9.plot(self.test_iteration,  self.test_scores["accuracy_s"], linestyle="--", label="Test Accuracy (smooth)")
        ax9.set_title("Accuracy globale au cours du temps"); ax9.set_xlabel("Itération"); ax9.set_ylabel("Accuracy")
        ax9.legend(); ax9.grid()

        fig3.tight_layout()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = PlotTabs()
    window.show()
    sys.exit(app.exec_())
