import numpy as np
import matplotlib.pyplot as plt
from control.matlab import *
from scipy import signal

plt.close('all')


## EXERCICE 1
G = tf([2,1],[1,2,1])
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
# Que le signal soit échantillonné ou pas, la réponse indicielle ne change pas.
# On peut en conclure que l'échantillonnage ne modifie en rien la réponse.


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
# doivent être compris dans le cercle unitaire, soit entre -1 et 1.

K=1
a=1;b=-1.2;c=0.32
H = tf([K],[a,b,c],0.1)
P = pole(H)
print('Les pôles de H sont:',P)

# Hb1 = H/(1+H)   # C'est faux
# print("Hb=",Hb1)
Hb2 = feedback(H,1)
print("Avec la fonction feedback, on a Hb=",Hb2)


K=0.67
H1 = tf([K],[a,b,c],0.1)
plt.figure()
rlist,klist = rlocus(H1)
plt.show()
# Le système stable car sur la figure, les points sont dans le cercle unitaire.
K=0.68
H2 = tf([K],[a,b,c],0.1)
plt.figure()
rlist,klist = rlocus(H2)
plt.show()
# Le système est stable pour les mêmes raisons que le précédent graphique.

y,T= step(feedback(H2,1))
plt.figure()
plt.plot(T,y)
plt.title("Réponse indicielles pour K=0.68")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.grid(True);plt.show()
K=0.6
H3 = tf([K],[a,b,c],0.1)
y,T= step(feedback(H3,1))
plt.figure()
plt.plot(T,y)
plt.title("Réponse indicielles pour K=0.6")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.grid(True);plt.show()

# Pour un K=0.68, on a un système oscillant.
# Pour un K=0.6, le système oscille dans un premier temps, puis le stabilise à environ 0.8.



## EXERCICE 4
print("\n\nEXERCICE 4\n")
H = tf([0.9,0],[1,-0.9],0.1)
y,T= step(feedback(H,1))
plt.figure()
plt.plot(T,y)
plt.title("Erreur de position du système")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.grid(True);plt.show()

Erreur = 1-y[-1]
print("L'erreur de position est",Erreur)



## EXERCICE 5
print("\n\nEXERCICE 5\n")
H = tf([10],[0.1,1])
Hd1 = c2d(H,0.1)
Hd2 = c2d(H,0.01)

Hdz1 = feedback(Hd1,1)
Hdz2 = feedback(Hd2,1)

print("Hdz1=",Hdz1)
# Le pôle de Hdz1 est égal à -5.953: il est inférieur à -1, le système n'est donc pas stable.
print("Hdz2=",Hdz2)
# Le pôle de Hdz2 est égal à -0.04679: il est compris dans [-1;1], le système est stable.


# D'après Shanon, la fréquence d'échantillonnage doit être deux fois supérieure à la fréquence du signal à échantillonné.
# Dans le cas de H, on a τ=0.1, donc la période d'échantillonnage doit être deux fois plus petite que τ.
# T_ech = 0.1/2 = 0.05. La période d'échantillonnage doit donc être égale ou inférieure à 0.05.
# T=0.1 est supérieur à 0.05: le critère n'est pas respecté.
# T=0.01 est inférieur à 0.05: le critère est respecté.
