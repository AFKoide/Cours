#%% Filtre
import numpy as np
import matplotlib.pyplot as plt

delta_t = 0.01
nombre_echantillons = 1000
temps = np.arange(0,nombre_echantillons)*delta_t

signal = 1.9*np.sin(5*temps)+ 1.5*np.sin(11*temps)+1.7*np.sin(2.3*temps)
signal = signal + np.sqrt(0.3)*np.random.randn(np.size(temps))

plt.plot(temps,signal)

# Filtrage du signal
def Filtre_Passebas(*argument):
    entree = argument[0]
    if len(argument) == 2:
        alpha = argument[1]
    else:
        alpha = 0.4

    i = 0
    filtrer = np.zeros(nombre_echantillons)
    for i in range(nombre_echantillons):
        if i == 0:
            filtrer[i] = alpha*entree[i]
        else: 
            filtrer[i] = alpha*entree[i] + (1-alpha)*filtrer[i-1]
    return filtrer

plt.plot(temps, Filtre_Passebas(signal,0.1))
plt.plot(temps, Filtre_Passebas(signal))
plt.show()
# Faire un filtre passe bas avec une procédure necessite de mettre le print dans la fonction : Une procédure est une fonction qui ne renvoie rien.

#%% Filtrage du signal
import numpy as np
import matplotlib.pyplot as plt

delta_t = 0.01
nombre_echantillons = 1000
temps = np.arange(0,nombre_echantillons)*delta_t

signal = 1.9*np.sin(5*temps)+ 1.5*np.sin(11*temps)+1.7*np.sin(2.3*temps)
signal = signal + np.sqrt(0.3)*np.random.randn(np.size(temps))

plt.plot(temps,signal)

def Filtre_Passebas_Causal(*argument):
    entree = argument[0]
    if len(argument) == 2:
        alpha = argument[1]
    else:
        alpha = 0.4
    i = 0
    filtrerC = np.zeros(nombre_echantillons)
    filtrerA = np.zeros(nombre_echantillons)
    filtrer = np.zeros(nombre_echantillons)

    for i in range(nombre_echantillons+1):
        j = i-1
        if i == 0:
            filtrerC[i] = alpha*entree[i]
            filtrer[i] = 1/(2-alpha)*filtrerC[i]-alpha*entree[i]
        elif i<1000:
            filtrerC[i] = alpha*entree[i] + (1-alpha)*filtrerC[j]
            filtrerA[j] = alpha*entree[j] + (1-alpha)*filtrerC[i]
            filtrer[i] = 1/(2-alpha)*filtrerC[i] + filtrerA[j] - alpha*entree[i]
        elif i == nombre_echantillons + 1:
            filtrerA[j] = alpha*entree[j] + (1-alpha)*filtrerC[i]
            filtrer[i] = 1/(2-alpha)*filtrerC[i]+filtrerC[j]-alpha*entree[i]
    return filtrer

plt.plot(temps, Filtre_Passebas_Causal(signal,0.1))

#%% LAMBDA
import numpy as np
import matplotlib.pyplot as plt
import scipy.optimize as resol
from math import pi

def Resolution(intervalle):
    comparer = np.zeros(intervalle*20)
    m=0
    f = lambda x: 0.8-(np.exp((-(x-3)**2-5)/100)-np.sin(pi*((x/30)+1)))
    n = np.arange(-intervalle*10,intervalle*10)*0.1 


    plt.plot(n,f(n))
    return f

f = Resolution(20)

print("Mafonction = 0 pour x = ",resol.fmin(f,20))
