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
# Signal aléatoire : pour un même échantillon, générer le même signal donnera des courbes différentes.

i = False
while i != True:
    signal = monscript.signal_aleatoire()
    if np.isnan(signal).any():
        print("Le signal contient des NaN. Regénération...")
        continue
    else:
        i += 1






"""2- Montrez que ce signal est stationnaire d’ordre 1."""
npoints = 1000

echantillon = 100
moyennes = np.zeros(npoints)
i = 0
signals = np.zeros((echantillon,npoints))
while i < echantillon:
    msignal = monscript.signal_aleatoire(npoints)
    if np.isnan(msignal).any():
        print("Le signal contient des NaN. Regénération...")
        continue
    signals[i] = msignal
    i += 1

moyennes = np.mean(signals,axis=0)

plt.subplot(3,1,1)
plt.plot(moyennes)

echantillon = 1000
moyennes = np.zeros(npoints)
i = 0
signals = np.zeros((echantillon,npoints))
while i < echantillon:
    msignal = monscript.signal_aleatoire(npoints)
    if np.isnan(msignal).any():
        print("Le signal contient des NaN. Regénération...")
        continue
    signals[i] = msignal
    i += 1

moyennes = np.mean(signals,axis=0)

plt.subplot(3,1,2)
plt.plot(moyennes)

echantillon = 10000
moyennes = np.zeros(npoints)
i = 0
signals = np.zeros((echantillon,npoints))
while i < echantillon:
    msignal = monscript.signal_aleatoire(npoints)
    if np.isnan(msignal).any():
        print("Le signal contient des NaN. Regénération...")
        continue
    signals[i] = msignal
    i += 1

moyennes = np.mean(signals,axis=0)

plt.subplot(3,1,3)
plt.plot(moyennes)
plt.show()

# Plus on a de signaux utilisé, plus l'écart type diminue donc on peut dire que l'on tend vers une valeur moyenne temporelle. 





"""3- Montrez que ce signal est ergodique d’ordre 1"""
# Signal Ergodique : moyenne temporelle d'une seule réalisation du signal = moyenne statistique de n réalisation du signal

# Moyenne statistique
echantillon = 10000
npoints = 10000
moyennes = np.zeros(npoints)
signals = np.zeros((echantillon,npoints))

i = 0
while i < echantillon:
    msignal = monscript.signal_aleatoire(npoints)
    if np.isnan(msignal).any():
        print("Le signal contient des NaN. Regénération...")
        continue
    signals[i] = msignal
    i += 1

moyennes_statistique = np.mean(signals,axis=0)
moyennes_temporel = np.mean(signals,axis=1)
plt.hist(moyennes_temporel,bins=1000)
plt.hist(moyennes_statistique,bins=1000)
plt.xlim([110,125]);plt.legend(["Moyennes Temporelle","Moyenne Statistique"])
plt.show()
# Ergodique car les deux histogrammes sont superposés.

# Loi des grands nombres : plus on a d'échantillon, plus on tend vers la bonne valeur.




"""4- Montrez que ce signal est stationnaire d’ordre 2"""
# Stationnaire d'ordre 2 : signal autocovariant
# 

Xi = signal
N = np.size(Xi)
k = 5
Xs = np.average(Xi)

def autocovariance(Xi, N, k, Xs):
    autoCov = 0
    for i in np.arange(0, N-k):
        autoCov += ((Xi[i+k])-Xs)*(Xi[i]-Xs)
    return (1/(N-1))*autoCov

print("Autocovariance:", autocovariance(Xi, N, k, Xs))



"""5- Montrez que ce signal est ergodique d’ordre 2,"""
# Signal ergodique : signal autocorrélation 
# Si 


"""6- Donnez la représentation AR de ce bruit en se limitant à un ordre 5."""

