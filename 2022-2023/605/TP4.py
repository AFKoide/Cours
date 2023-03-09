import numpy as np
import matplotlib.pyplot as plt
from control.matlab import *
from scipy import signal

plt.close('all')


## EXERCICE 1
G = tf([0,2,1],[1,2,1])
y,T = c2d(G,0.1)

plt.figure()
plt.plot(Gd)
plt.grid(True)
plt.show()


## EXERCICE 2
Hd = tf([0.047,0.046],[1,-1.81,0.9])
z,p,k = tf2zpk([0.047,0.046],[1,-1.81,0.9])
print("Zéros: ",z,"\tPôles: ",p,"\tGain: ",k)

pzmap(Hd,plot=True)
plt.grid(True)
plt.show()


## EXERCICE 3
# La condition de stabilité d'un système échantillonné linéaire est les pôles doivent 
a=1;b=-1.2;c=0.32
delta=2**2-4*a*c
p1=(-b+(delta)**(1/2))/(2*a);print("P1: ",p1)
p2=(-b-(delta)**(1/2))/(2*a);print("P2: ",p2)

H = tf([1],[a,b,c])
print(H)
y,T= step(feedback(H,1))
plt.plot(T,y)

plt.title("Réponse Boucle Fermée")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.legend(["Hb","Fonction Feedback"])
plt.grid(True);plt.show()


## EXERCICE 4
Hd = tf([0.9,],[1,-0.9])
y,T= step(feedback(Hd,1))
print("Erreur de position: ",y[-1])


## EXERCICE 5
H = tf([10],[0.1,1])
