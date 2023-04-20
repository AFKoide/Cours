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



# EXERCICE 2
print("\n\nEXERCICE 2: Correcteur ZDAN\n")

# Ft théorique
Ht = tf([1],[1/4,0.707,1])
Gtz= c2d(Ht,1)
print('La fonction de transfert continue théorique Ht(p) vaut ',Ht)
print('La fonction de transfert discrète théorique Gt(z) vaut ',Gtz)


Ca = tf([1,-0.3679],[1,0.7189],1)
Cb = tf([1,0],[1,-1],1)
Cc = tf([5.231,-2.56],[1,0],1)
C = Ca*Cb*Cc
Gcorrige = feedback(C*G,1)

plt.figure()
y,T= step(C*G)
plt.plot(T,y)
plt.title("Réponse Indicielle - Boucle ouverte")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.grid(True);plt.show()

plt.figure()
y,T= step(Gcorrige)
plt.plot(T,y)
plt.title("Réponse Indicielle - Boucle fermée")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.grid(True);plt.show()

# On peut observer que la réponse en boucle fermée a une erreur de position nulle et un temps de réponse extrèmement faible.
# Cependant, on a un dépassement très important.
