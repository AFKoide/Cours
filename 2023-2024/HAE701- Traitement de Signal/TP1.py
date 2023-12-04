import sys
sys.path.insert(1, './Script')
import monscript
import numpy as np
import matplotlib.pyplot as plt 
from scipy.stats import kstest



def Kolmogorov_Smirnov(x,y):
    xx, xy, yx, yy = 0, 0, 0, 0
    
    for i in range(0,len(x)):
        xx += 1 if x[i] <= x[i] else 0
        if len(y) <= i:
            xy += 1
        else:
            xy += 1 if y[i] <= x[i] else 0

    for i in range(len(y)):
        if len(x) <= i:
            yx += 1
        else:
            yx += 1 if x[i] <= y[i] else 0
        yy += 1 if y[i] <= y[i] else 0

    Obs_x = abs(xx - xy); print("Obs_x:",Obs_x)
    Obs_y = abs(yx - yy); print("Obs_y:",Obs_y)
    KS = max(Obs_x, Obs_y);print("KS=",KS)

    if KS > 1.66*(1/len(x) + 1/len(y)):
        print(KS)
        print('On peut rejeter l’hypothèse d’égalité des moyennes car KS=',KS,'>',1.66*(1/len(x) + 1/len(y)))
    else:
        print('On ne peut pas rejeter l’hypothèse d’égalité des moyennes car',KS,'<=',1.66*(1/len(x) + 1/len(y)))



def Mahalanobis(x,y):
    if len(x) != len(y):
        print("Les deux séries d’échantillons n'ont pas la même taille :",len(x),len(y))
        return None

    moy_x = np.mean(x); var_x = np.var(x)
    moy_y = np.mean(y); var_y = np.var(y)

    D = (moy_x-moy_y)**2/(var_x**2+var_y**2)
    if D > 3.81:
        print('On peut rejeter l’égalité des deux moyennes (dans 5% des cas): D=',D)




"""1- Vérifiez que ce signal est bien aléatoire,"""
i = False
while i != True:
    signal = monscript.signal_aleatoire()
    if np.isnan(signal).any():
        print("Le signal contient des NaN. Regénération...")
        continue
    else:
        i = True

# Générer un signal avec une distribution normale
moyenne = 0
ecart_type = 1
taille_de_l_echantillon = 1000

signal_normale = np.random.normal(moyenne, ecart_type, taille_de_l_echantillon)
plt.plot(signal);plt.plot(signal_normale)
plt.legend(["Strauss","Numpy"])
plt.show()

Kolmogorov_Smirnov(signal,signal_normale)

# statistic, p_value = kstest(signal,'norm',N=1000)
# 
# print(f"Statistique du test : {statistic}")
# print(f"P-value : {p_value}")
# 
# alpha = 0.05
# if p_value > alpha:
#     print("Le signal semble être aléatoire.")
# else:
#     print("Le signal ne semble pas être aléatoire.")




"""2- Montrez que ce signal est stationnaire d’ordre 1."""
# Signal Stationnaire : moyenne et variance constante dans le temps 

moyenne_signal = np.mean(signal)
variance_signal = np.var(signal)

# Vérifier la constance de la moyenne et de la variance
stationnaire_ordre_1 = np.allclose(moyenne_signal, np.mean(signal)) and np.allclose(variance_signal, np.var(signal))

if stationnaire_ordre_1:
    print("Le signal est stationnaire d'ordre 1.")
else:
    print("Le signal n'est pas stationnaire d'ordre 1.")




"""3- Montrez que ce signal est ergodique d’ordre 1"""
# Signal Ergodique : moyenne temporelle d'une seule réalisation du signal = moyenne statistique de n réalisation du signal
# Moyenne temporelle 
moyenne_temporelle = moyenne_signal

# Moyenne statistique
echantillon = 4
tolerance = 1e-6

moyennes = np.zeros(echantillon)

i = 0
while i < echantillon:
    msignal = monscript.signal_aleatoire()
    if np.isnan(msignal).any():
        print("Le signal contient des NaN. Regénération...")
        continue
    moyennes[i] = np.mean(msignal)
    i += 1

moyenne_statistique = np.mean(moyennes)
ergodicite_ordre_1 = np.isclose(moyenne_temporelle, moyenne_statistique, rtol=tolerance)

print(f"La moyenne temporelle du signal est égal à {moyenne_temporelle} tandis que la moyenne statistique est de {moyenne_statistique}")
if ergodicite_ordre_1:
    print("Le signal est ergodique d'ordre 1.")
else:
    print("Le signal n'est pas ergodique d'ordre 1.")


"""4- Montrez que ce signal est stationnaire d’ordre 2,"""
"""5- Montrez que ce signal est ergodique d’ordre 2,"""
"""6- Donnez la représentation AR de ce bruit en se limitant à un ordre 5."""

