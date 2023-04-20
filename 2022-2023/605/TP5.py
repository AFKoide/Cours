import numpy as np
import matplotlib.pyplot as plt
from control.matlab import *
from scipy import signal

plt.close('all')


## EXERCICE 1
print("\n\nEXERCICE 1: Système linéaire continue\n")

num = [1]
den = [1, 1, 0]
H = tf (num, den)
G = c2d(H, 1)
print('La fonction de transfert Gd(z) vaut ', G)

y,T= step(feedback(G,1))
plt.plot(T,y)
plt.title("Réponse Indicielle")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.grid(True);plt.show()



# EXERCICE 2
print("\n\nEXERCICE 2: Correcteur ZDAN\n")

# Ft théorique
Ht = tf([1],[1/4,0.707,1])
Gtz= c2d(Ht,1)
print('La fonction de transfert continue théorique Ht(p) vaut ',Ht)
print('La fonction de transfert discrète théorique Gt(z) vaut ',Gtz)
