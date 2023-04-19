import numpy as np
import matplotlib.pyplot as plt
from control.matlab import *
from scipy import signal
from math import sqrt

plt.close('all')


## EXERCICE 1
G = tf([0,2,1],[1,2,1])
Gz = c2d(G,0.1)

plt.figure()
y,T= step(feedback(G,1))
plt.plot(T,y)
y,T= step(feedback(Gz,1))
plt.plot(T,y)
plt.title("Comparaison entre système et système échantillonné")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.legend(["Système classique","Système échantillonné"])
plt.grid(True);plt.show()



# EXERCICE 2
print("\n\nEXERCICE 2\n")
H = tf([0.047,0.046],[1,-1.81,0.9])
Hd = c2d(H,0.1)
z,p,k = tf2zpk([0.047,0.046],[1,-1.81,0.9])
print("Zéros: ",z,"\tPôles: ",p,"\tGain: ",k)

plt.figure()
pzmap(Hd,plot=True)
plt.title("Pôles de Hd")
plt.grid(True);plt.show()

P = pole(Hd)
print("Les Pôles de Hd sont:",P)



## EXERCICE 3
print("\n\nEXERCICE 3\n")
# La condition de stabilité d'un système échantillonné est que les modules des pôles
# doivent être compris dans le cercle unitaire,soit entre -1 et 1.

K=1
a=1;b=-1.2;c=0.32
H = c2d(tf([K],[a,b,c]),0.1)
P = pole(H)
print('Les pôles de H sont:',P)

Hb1 = H/(1+H)
print("Hb=",Hb1)
Hb2 = feedback(H,1)
print("Avec la fonction feedback, on a Hb=",Hb2)

plt.figure()
rlist,klist = rlocus(Hb2)
plt.grid();plt.show()

# Le système n'est pas stable car le module des pôles est supérieur à 1.

    
## EXERCICE 4
print("\n\nEXERCICE 4\n")
H = tf([0.9,],[1,-0.9])
Hd = c2d(H,0.01)
y,T= step(feedback(Hd,1))
plt.figure()
plt.plot(T,y)
plt.title("Erreur de position du système")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.grid(True);plt.show()

## EXERCICE 5
print("\n\nEXERCICE 5\n")
H = tf([10],[0.1,1])
Hd1 = c2d(H,0.1)
Hd2 = c2d(H,0.01)
