import sys
sys.path.insert(1, './Script')
import monscript
import numpy as np
import matplotlib.pyplot as plt 
from scipy.stats import kstest

"""On génère tous les signaux"""
nbpoints = 1000

signals_10 = np.zeros((10,nbpoints))
signals_100 = np.zeros((100,nbpoints))
signals_1000 = np.zeros((1000,nbpoints))
signals_10000 = np.zeros((10000,nbpoints))

plt.subplot(4,1,1)
plt.title("10 signaux générés")
i = 0
while i != 10:
    signal = monscript.signal_aleatoire(nbpoints)
    if np.isnan(signal).any():
        print("Le signal contient des NaN. Regénération...")
        continue
    plt.plot(signal)
    signals_10[i] = signal
    i += 1

plt.subplot(4,1,2)
plt.title("100 signaux générés")
i = 0
while i != 100:
    signal = monscript.signal_aleatoire(nbpoints)
    if np.isnan(signal).any():
        print("Le signal contient des NaN. Regénération...")
        continue
    plt.plot(signal)
    signals_100[i] = signal
    i += 1

plt.subplot(4,1,3)
plt.title("1000 signaux générés")
i = 0
while i != 1000:
    signal = monscript.signal_aleatoire(nbpoints)
    if np.isnan(signal).any():
        print("Le signal contient des NaN. Regénération...")
        continue
    plt.plot(signal)
    signals_1000[i] = signal
    i += 1

plt.subplot(4,1,4)
plt.title("10000 signaux générés")
i = 0
while i != 10000:
    signal = monscript.signal_aleatoire(nbpoints)
    if np.isnan(signal).any():
        print("Le signal contient des NaN. Regénération...")
        continue
    plt.plot(signal)
    signals_10000[i] = signal
    i += 1
plt.show()

"""1- Vérifiez que ce signal est bien aléatoire,"""
# Signal aléatoire : pour un même échantillon, générer le même signal donnera des courbes différentes.

print(f"Plage pour un pas de 10 est {np.mean(np.ptp(signals_10, axis=0))}, pour 100 est {np.mean(np.ptp(signals_100, axis=0))}, pour 1000 est {np.mean(np.ptp(signals_1000, axis=0))}, pour 10000 est {np.mean(np.ptp(signals_10000, axis=0))}")
