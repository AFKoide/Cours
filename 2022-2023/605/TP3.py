import numpy as np
import matplotlib.pyplot as plt
from control.matlab import *



J=0.01;C=0.004;K=10;Ki=0.05;

H = tf(Ki,[J,C,K])
print(H)

#Determination de la réponse indicielle
#y represente la sortie et T le vecteur temps
y,T = step(H)

plt.plot(T,y)
plt.title("Réponse Indicielle du système")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.grid(True);plt.show()


# CORRECTEUR 1
wn=76
e=0.707

z,p,k = tf2zpk(wn**2,[1,2*e*wn,wn**2])
print("Zéros: ",z,"\tPôles: ",p,"\tGain: ",k)

a=6985
b=35.5
c=389

C1 = a*tf([1,b],[1,c])
print(C1)

S = feedback(C1*H,1)
print(S)

y,T = step(S)

plt.plot(T,y)
plt.axhline(1,color='k',linestyle='--')
plt.axhline(y[-1]+0.05,color='g',linestyle='-.')
plt.title("Réponse Indicielle du système avec correcteur 1")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.grid(True);plt.show()

print("Erreur de position: ",1-y[-1])
print("Le temps de réponse est: ",)

# CORRECTEUR 2
a=1
b=15.37
c=1

C2 = a*tf([1,b],[1,c])
print(C2)

S2 = feedback(C2*C1*H,1)
print(S2)

y,T = step(S2)

plt.plot(T,y)
plt.axhline(1,color='k',linestyle='--')
plt.axhline(y[-1]+0.05,color='g',linestyle='-.');plt.axhline(y[-1]-0.05,color='g',linestyle='-.')
plt.title("Réponse Indicielle du système avec le correcteur 1 & 2")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.grid(True);plt.show()

print("Erreur de position: ",1-y[-1])
print("Le temps de réponse est: ",)

# CORRECTEUR 3
a=1
b=20
c=0

C3 = a*tf([1,b],[1,c])
print(C3)

S3 = feedback(C3*C1*H,1)
print(S3)

y,T = step(S3)

plt.plot(T,y)
plt.axhline(1,color='k',linestyle='--')
plt.axhline(y[-1]+0.05,color='g',linestyle='-.');plt.axhline(y[-1]-0.05,color='g',linestyle='-.')
plt.title("Réponse Indicielle du système avec correcteur 1 & 3")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.grid(True);plt.show()

print("Erreur de position: ",1-y[-1])
print("Le temps de réponse est: ",)
