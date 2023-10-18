#%% Histogramme
import math as m
import numpy as np
import matplotlib.pyplot as plt


def Histogramme(*args):# data, intervalle, p
    data = args[0]
    if len(args) == 3:
        intervalle = args[1]
        p = args[2]
    elif len(args) == 2:
        intervalle = args[1]
        p = 50
    else:
        intervalle = [min(data),max(data)]
        p = 50
    
    
    
    Histo = np.zeros(p)
    Mode = np.zeros(p)
    imin = intervalle[0];imax = intervalle[1]
    taille = (imax-imin)/p
    
    
    
    for n in range(p):
        Mode[n] = imin+taille*(1+n)
    
    for valeur in data:
        for n in range(p):
            # print("valeur de n:",n)
            if valeur <= Mode[n]:
                Histo[n] += 1
                break
            #else:
                # print(valeur,"est supérieur à",taille*n)
    return Mode, Histo

N = [9, 10, 4, 2, 2, 4, 1, 4, 7, 4, 3, 3, 2, 2, 4, 4, 9, 8, 5, 6, 3, 3, 2, 10, 10, 9, 5, 5, 5, 8, 4, 7, 10, 9, 8, 9, 4, 7, 3, 10]
N2 = [1,2,3,4,5]
intervaller = [1,5]


Mode, Histo = Histogramme(N)

plt.bar(Mode, Histo,0.1)


Fichier_de_donnee = open('Echantillons.txt','r')
data_du_fichier = Fichier_de_donnee.readlines()
X = [float(element) for element in data_du_fichier]
Fichier_de_donnee.close()

Mode, Histo = Histogramme(X)

plt.bar(Mode, Histo,0.1)
