from PyQt5.QtWidgets import QApplication, QTabWidget, QWidget, QVBoxLayout, QMainWindow, QPushButton, QHBoxLayout
import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import savgol_filter
from sklearn.metrics import confusion_matrix, ConfusionMatrixDisplay, classification_report, accuracy_score
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from PyQt5.QtWidgets import QApplication, QTabWidget, QWidget, QVBoxLayout, QMainWindow
import sys
import re

LINEAR = 0
GAME = 1
MACSUM = 2
MACSUM_SANS_TOL = 3


RAPPORT = False


# Charger les fichiers de logs
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
        if len(signal) < window_length:
            window_length = len(signal) if len(signal) % 2 == 1 else len(signal) - 1
        return savgol_filter(signal, window_length=window_length, polyorder=polyorder)

    def refresh(self):
        self.tabs.clear()
        self.init_tabs()

    def load_logs(self):
        global test_log, training_log
        try:
            test_log = pd.read_csv(test_log_path)
            training_log = pd.read_csv(training_log_path)
            training_log.columns = training_log.columns.str.strip()
            test_log.columns = test_log.columns.str.strip()
            # Stocker les colonnes du test_log
            self.test_iteration = test_log["iteration"].to_numpy()
            self.test_erreur_moyenne = test_log["erreur_moy"].to_numpy()
            self.test_nombre_egalite = test_log["nombre_egalite"].to_numpy()
            self.test_nombre_preference = test_log["nombre_preference"].to_numpy()
            self.test_pred_vrai_egalite = test_log["pred_vrai_egalite"].to_numpy()
            self.test_pred_fausse_egalite = test_log["pred_fausse_egalite"].to_numpy()
            self.test_pred_vrai_preference = test_log["pred_vrai_preference"].to_numpy()
            self.test_pred_fausse_preference = test_log["pred_fausse_preference"].to_numpy()
            self.test_pred_fausse_inversion = test_log["pred_fausse_inversion"].to_numpy()

            # Stocker les colonnes du training_log
            self.train_iteration = training_log["iteration"].to_numpy()
            self.train_erreur_moyenne = training_log["erreur_moy"].to_numpy()
            self.train_nombre_egalite = training_log["nombre_egalite"].to_numpy()
            self.train_nombre_preference = training_log["nombre_preference"].to_numpy()
            self.train_pred_vrai_egalite = training_log["pred_vrai_egalite"].to_numpy()
            self.train_pred_fausse_egalite = training_log["pred_fausse_egalite"].to_numpy()
            self.train_pred_vrai_preference = training_log["pred_vrai_preference"].to_numpy()
            self.train_pred_fausse_preference = training_log["pred_fausse_preference"].to_numpy()
            self.train_pred_fausse_inversion = training_log["pred_fausse_inversion"].to_numpy()
        except Exception as e:
            print(f"Erreur lors du chargement ou du traitement des logs : {e}")



    def init_tabs(self):
        self.load_logs()
        # Tab 1: Courbes
        tab1 = QWidget()
        layout1 = QVBoxLayout()
        fig1 = Figure(figsize=(14, 6))
        canvas1 = FigureCanvas(fig1)
        layout1.addWidget(canvas1)
        tab1.setLayout(layout1)
        self.tabs.addTab(tab1, "Courbes")

        # Courbes
        ax1 = fig1.add_subplot(3, 2, 1)
        ax1.plot(self.train_iteration, self.train_erreur_moyenne, label="Erreur Moyenne", color='blue')
        ax1.set_title("Logs de Training")
        ax1.set_xlabel("Epoch")
        ax1.set_ylabel("Erreur Moyenne")
        ax1.legend()
        ax1.grid()
        
        ax2 = fig1.add_subplot(3, 2, 2)
        ax2.plot(self.test_iteration, self.test_erreur_moyenne, label="Erreur Moyenne", color='green')
        ax2.set_title("Logs de Test")
        ax2.set_xlabel("Itération")
        ax2.set_ylabel("Erreur Moyenne")
        ax2.legend()
        ax2.grid()
        

        ax3 = fig1.add_subplot(3, 2, 3)
        vraies_egalites = self.train_pred_vrai_egalite / self.train_nombre_egalite * 100
        fausses_egalites = self.train_pred_fausse_egalite / self.train_nombre_egalite * 100

        smoothed_vraies = PlotTabs.apply_savgol(vraies_egalites)
        smoothed_fausses = PlotTabs.apply_savgol(fausses_egalites)
        
        ax3.plot(self.train_iteration[:len(smoothed_vraies)], smoothed_vraies, label="Vraies égalités (%)", color='blue', linestyle="-")
        ax3.plot(self.train_iteration[:len(smoothed_fausses)], smoothed_fausses, label="Fausses égalités (%)", color='red', linestyle="--")
        ax3.set_xlabel("Epoch")
        ax3.set_ylabel("Pourcentage")
        ax3.legend()
        ax3.grid()
        

        ax4 = fig1.add_subplot(3, 2, 5)
        vraies_pref = self.train_pred_vrai_preference / self.train_nombre_preference * 100
        fausses_pref = self.train_pred_fausse_preference / self.train_nombre_preference * 100
        fausses_inv = self.train_pred_fausse_inversion / self.train_nombre_preference * 100

        smoothed_vraies =  PlotTabs.apply_savgol(vraies_pref)
        smoothed_fausses = PlotTabs.apply_savgol(fausses_pref)
        smoothed_inverse = PlotTabs.apply_savgol(fausses_inv)

        ax4.plot(self.train_iteration, smoothed_vraies, label="Vraies préférences (%)", color='blue', linestyle="-")
        ax4.plot(self.train_iteration, smoothed_fausses, label="Fausses préférences (%)", color='red', linestyle="--")
        ax4.plot(self.train_iteration, smoothed_inverse, label="Fausses inversions (%)", color='purple', linestyle="--")
        ax4.set_xlabel("Epoch")
        ax4.set_ylabel("Pourcentage")
        ax4.legend()
        ax4.grid()
        

        ax5 = fig1.add_subplot(3, 2, 4)
        vraies_egalites = self.test_pred_vrai_egalite / self.test_nombre_egalite * 100
        fausses_egalites = self.test_pred_fausse_egalite / self.test_nombre_egalite * 100
        
        smoothed_vraies = PlotTabs.apply_savgol(vraies_egalites)
        smoothed_fausses = PlotTabs.apply_savgol(fausses_egalites)
        
        ax5.plot(self.test_iteration[:len(smoothed_vraies)], smoothed_vraies, label="Vraies égalités (%)", color='blue', linestyle="-")
        ax5.plot(self.test_iteration[:len(smoothed_fausses)], smoothed_fausses, label="Fausses égalités (%)", color='red', linestyle="--")
        ax5.set_xlabel("Itération")
        ax5.set_ylabel("Pourcentage")
        ax5.legend()
        ax5.grid()
        

        ax6 = fig1.add_subplot(3, 2, 6)
        vraies_pref = self.test_pred_vrai_preference / self.test_nombre_preference * 100
        fausses_pref = self.test_pred_fausse_preference / self.test_nombre_preference * 100
        fausses_inv = self.test_pred_fausse_inversion / self.test_nombre_preference * 100
        
        smoothed_vraies =  PlotTabs.apply_savgol(vraies_pref)
        smoothed_fausses = PlotTabs.apply_savgol(fausses_pref)
        smoothed_inverse = PlotTabs.apply_savgol(fausses_inv)

        ax6.plot(self.test_iteration, smoothed_vraies, label="Vraies préférences (%)", color='blue', linestyle="-")
        ax6.plot(self.test_iteration, smoothed_fausses, label="Fausses préférences (%)", color='red', linestyle="--")
        ax6.plot(self.test_iteration, smoothed_inverse, label="Fausses inversions (%)", color='purple', linestyle="--")
        ax6.set_xlabel("Itération")
        ax6.set_ylabel("Pourcentage")
        ax6.legend()
        ax6.grid()
        fig1.tight_layout()






        # Tab 2: Matrices de confusion et rapports
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
        ])
        y_pred_train = y_pred_train[:len(y_true_train)]
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
        ])
        y_pred_test = y_pred_test[:len(y_true_test)]
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




if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = PlotTabs()
    window.show()
    sys.exit(app.exec_())
