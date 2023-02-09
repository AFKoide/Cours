import numpy as np
import matplotlib.pyplot as plt
from control.matlab import *
from scipy import signal

num = 100
den = [1,10,29,20]

H = tf(num,den)
print(H)

z,p,k = tf2zpk(num,den)
print("Zéros: ",z,"\tPôles: ",p,"\tGain: ",k)



#Determination de la réponse indicielle
#y represente la sortie et T le vecteur temps
T, yout = signal.impulse((num,den))

plt.plot(T,yout)
plt.title("Réponse Indicielle du système")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.grid(True);plt.show()
