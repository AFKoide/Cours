"GOOGLE COLAB : mettre au début du code : !pip install control"
"Ca permet d'installer le control."

#%% Introduction
import numpy as np
import matplotlib.pyplot as plt
from control.matlab import *
plt.close('all')

def MCC(R,L,Ke,Km,Fv,J):
    A = np.array([[-R/L,-Ke/L,0],[Km/J,-Fv/J,0],[0,1,0]])
    B = np.array([[1/L],[0],[0]])
    C = np.array([[0,0,1]])
    D = np.array([[0]])
    sys_ss = ss(A,B,C,D)
    print(sys_ss)
    
    y,T = step(sys_ss)
    plt.figure();plt.xlabel("Le système est instable.")
    plt.plot(T,y);plt.grid('on');plt.title("Angle du Moteur")
    "QUESTION 1: La matrice C est égale à [0,0,1] et D = 0"
    
    H = ss2tf(sys_ss)
    print("La fonction de transfert du moteur est",H)
    poles = pole(H)
    print("Les pôles de H sont:",poles)
    "QUESTION 4: un des pôle de H est nul, donc le système instable car pas strictement négatif."
    
    
    commandabilite = ctrb(A,B)
    print("La commandabilité est:",np.linalg.det(commandabilite),"le système est donc commandable car != 0")
    
    
    K = place(A,B,[-10,-20,-100])
    print(K)

    Ac = [[-R/L,-Ke/L,0],[Km/J,-Fv/J,0],[0,1,0]]
    Bc = [[1/L],[0],[0]]
    Cc = [[0,0,1]] 
    Dc = [[1]]
    sysc_ss = ss(A-B@K,B,C-D@K,D)
    print(sysc_ss)
    y,T = step(sysc_ss)
    plt.figure();plt.xlabel("Avec le correcteur, le système est stable mais a une importante erreur de position.")
    plt.plot(T,y);plt.grid('on');plt.title("Angle du Moteur")

    k1 = 1
    Achapeau = np.block([[A,np.zeros((3, 1))],[-C,np.zeros((1, 1))]])
    Bchapeau = np.block([[B],[0]])
    Kchapeau = np.block([K,-k1])
    Cchapeau = np.block([[C,0]])

    syscchapeau_ss = ss(Achapeau-Bchapeau@Kchapeau,np.array([[0],[0],[0],[1]]),Cchapeau,D)
    print(syscchapeau_ss)
    y,T = step(syscchapeau_ss)
    plt.figure();plt.xlabel("Le système est plus lent qu'avec seulement le correcteur P, mais l'erreur de position est nulle")
    plt.plot(T,y);plt.grid('on');plt.title("Angle du Moteur")
    

if __name__ == '__main__':
    plt.close('all')
    sys_ss = MCC(1,4e-3,0.1,0.1,3e-3,2e-3)


#%% Commande lineaire quadratique d’une voiture controĺee en acceleration
import numpy as np
import matplotlib.pyplot as plt
from control.matlab import *
plt.close('all')

A = np.array([[0,1],[0,0]])
B = np.array([[0],[1]])
C = np.array([[1,0]])
D = np.array([0])

sys_ss = ss(A,B,C,D)

H = ss2tf(sys_ss)
pole = pole(H)
print("La fonction de transfert du système est:",H,"Et ses pôles sont",pole)
"QUESTION 10: les pôles de H sont nuls. Le système est donc instable."

y,T = step(sys_ss)
plt.figure()
plt.plot(T,y);plt.grid('on');plt.title("Angle du Moteur")

CO = ctrb(A,B)
print("La commandabilité est:",np.linalg.det(CO))
"QUESTION 12: La voiture est commandable puisque le déterminant de CO n'est pas nul."



def dessinePosVelAcc(A,B,C,D):
    voitP = ss2tf(A,B,C,D)
    C = np.array([0,1])
    voitV = ss2tf(A,B,C,D)
    pos,T = step(voitP)
    vel,T = step(voitV)
    acc,T = impulse(voitV)
    plt.figure()
    plt.plot(T,pos)
    plt.plot(T,vel)
    plt.plot(T,acc)
    plt.legend(['pos(t)','vel(t)','acc(t)'])
    plt.xlabel("temps")
    plt.grid('on');plt.show()

Q = np.eye(2); R = np.eye(1)
K, S, E = lqr(A, B, Q, R)

dessinePosVelAcc(A-B@K,B,C-D@K,D)
"Observation de la courbe: le système doit se mettre en mouvement donc l'acceleration est importante au début."
"Quand le système accélère, la vélocité augmente et la position également, et l'accélération diminue."
"Quand on approche de la valeur finale prévue, l'accélération devient négative : on freine."
"Freiner fait diminuer la vitesse, et donc la variation de la position. A la valeur de finale de la position demandée, la vitesse et l'accélération sont nulles."


# K1 = 1 * B.T @ S
K = place(A,B,E)
dessinePosVelAcc(A-B@K,B,C-D@K,D)
"QUESTION 14: En utilisant la fonction place, on obtient les mêmes courbes :"
"La fonction lqr renvoie les valeurs propres du système, que l'on utilise pour déterminer K."
