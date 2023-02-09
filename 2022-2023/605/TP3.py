import numpy as np
import matplotlib.pyplot as plt
from control.matlab import *

H = tf(1,[0.01,4e-3,10])
print(H)

#Determination de la réponse indicielle
#y represente la sortie et T le vecteur temps
y,T = step(H)

plt.plot(T,y)
plt.title("Réponse Indicielle du système")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.grid(True);plt.show()