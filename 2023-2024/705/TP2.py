import numpy as np
import matplotlib.pyplot as plt

delta_t = 0.01
nombre_echantillons = 1000
temps = np.arange(0,nombre_echantillons)*delta_t

signal = 1.9*np.sin(5*temps)+ 1.5*np.sin(11*temps)+1.7*np.sin(2.3*temps)
signal = signal + np.sqrt(0.3)*np.random.randn(np.size(temps))

plt.plot(temps,signal)


# Filtrage du signal
def Filtre(entree, alpha):
    filtrer = np.zeros(nombre_echantillons)
    for i in filtrer:
        
        filtrer[i] = alpha*entree[i] + (1-alpha)*filtrer[i-1];
    return filtrer;


plt.plot(temps, Filtre(signal,0.1))