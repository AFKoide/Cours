#%% Introduction
import numpy as np
import matplotlib.pyplot as plt
from control.matlab import *
plt.close('all')

#fonction de transfert H(s)=$1/(s^2+2s+3)$
num = [1.0]
den = [1.0,2.0,3.0]
H = tf(num,den)
print("La fonction de transfert H est egale a:",H)
#evaluation de la reponse indicielle
#y represente la sortie et T le vecteur temps
y,T = step(H,linspace(0,5,500))
#affichage du resultat de la reponse indicielle
plt.figure()
plt.plot(T.T,y.T)
plt.show()

#%% Simulation d’un système continu
import numpy as np
import matplotlib.pyplot as plt
from control.matlab import *
"Question 1: i(t) est le courant consommé par le moteur (entrée), w(t) est la vitesse de rotation du moteur (sortie) et teta(t) est l'angle de sortie du moteur (sortie)."
"Ces trois variables sont très importante pour la "


# Question 3
def MCC():
    R=1.52;L=2.2e-3;Ke=0.127;Km=Ke;Fv=5.06e-5;J=8.3e-5
    
    A = [[-R/L,-Ke/L,0],[Km/J,-Fv/J,0],[0,1,0]]
    B = [[1/L],[0],[0]]
    C = [[0,0,1]] 
    D = [[0]]
    sys_ss = ss(A,B,C,D)
    print(sys_ss)
    y,T = step(sys_ss)
    plt.figure()
    plt.plot(T,y);plt.grid('on');plt.title("Angle du Moteur")
    
    C = [[0,1,0]] 
    sys_ss = ss(A,B,C,D)
    y,T = step(sys_ss)
    plt.figure()
    plt.plot(T,y);plt.grid('on');plt.title("Vitesse Angulaire")
    sys_tf = tf(sys_ss)
    print(sys_tf)
    "Question 5: on observe que la fonction de transfert du moteur qui relie"
    "u(t) et w(t) est du second ordre."    
    
    C = [[1,0,0]] 
    sys_ss = ss(A,B,C,D)
    y,T = step(sys_ss)
    plt.figure()
    plt.plot(T,y);plt.grid('on');plt.title("Courant");plt.show()
    "Question 4: Au démarrage, le moteur a besoin de beaucoup de puissance pour passer"
    "d'un état statique à rotationnel, d'où le pic de courant."
    
    

if __name__ == '__main__':
    plt.close('all')
    MCC()

#%% Simulation des systèmes numériques
import numpy as np
import matplotlib.pyplot as plt
from control.matlab import *

print("Partie 3: Simulation des systèmes numériques\n\n")

"Question 6: Les grandeurs à connaitre pour résoudre l'équation (6) sont y(k) pour k=0, 1 et 2"

# Question 7 & 8
def equ2(N):
    y = np.zeros(N+2)
    u = np.ones(N)
    for k in range(N):
        y[k+2] = u[k] - (0.7*y[k] - 0.1*y[k+1])
    plt.plot(y);plt.grid("on");plt.title("Equation Récurrente")

# Question 9
def equtf():
    num = [1]
    den = [1,-0.1,0.7]
    H = tf(num,den,1)
#    print('Fonction de Transfert:',H)
    y,T = step(H)
    plt.figure()
    plt.plot(T.T,y.T);plt.grid("on");plt.title("TF Discrète")
    return H

# Question 10
def equvec():
    A = [[0,1],[-0.7,0.1]]
    B = [[0],[1]]
    C = [[0,1]]
    D = [[0]]
    R1 = ss(A,B,C,D,1)
#    print("Equation d'état:",R1)
    y,T = step(R1)
    plt.figure()
    plt.plot(T,y);plt.grid('on');plt.title("Représentation d'état R1")
    return R1


# Appel des fonctions
equ2(40)
H = equtf()
R1 = equvec()

# Question 11
TF = ss2tf(R1)
print("Fonction de Transfert issue de la Représentation d'état R1:",TF)

"On peut observer que la fonction de transfert issue de la représentation d'état est différente de la fonction de transfert H."

# Question 12
R2 = tf2ss(H)
print("Représentation d'état R2 issue de la Fonction de Transfert :\n",R2)

y1, T1 = step(R1,40)
y2, T2 = step(R2,40)

plt.figure()
plt.plot(T1,y1);plt.plot(T2,y2)
plt.grid('on');plt.title("Comparaison de R1 et R2");plt.legend(["R1","R2"]);plt.show()

"On peut observer un décalage entre le signal R1 et R2 d'environ 1 : cela s'explique par l'initialisation du signal R1 qui est nul à t=0 tandis que l'équation d'état est déjà initialisé."