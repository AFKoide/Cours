#%% CURTIS MARTELET
import numpy as np
import matplotlib.pyplot as plt
from control.matlab import *

def system():
    A = np.array([[0,0,1],[1,-1,2],[-6,0,-5]])
    B = np.array([[1],[2],[1]])
    C = np.array([[1,1,1]])
    D = 0

    sys_ss = ss(A,B,C,D)

    y,T = step(sys_ss)
    plt.figure()
    plt.plot(T,y);plt.grid('on');plt.show()
    return sys_ss
    """
    Si toutes les valeurs propres de M ont une partie réelle strictement négative,
    alors l'erreur du système converge vers zéro à mesure que le temps avance.

    Comme vu sur le tracé, la réponse converge bien.
    """

def OKT(sys_ss):
    At = sys_ss.A.transpose()
    Ct = sys_ss.C.transpose()
    Lt = place(At,Ct,[-4,-5,-6])
    L = Lt.transpose()
    print("\nMatrice L:\n",L)

    # Vérification:
    print(sys_ss.A - L @ sys_ss.C)
    return L



if __name__ == '__main__':
    sys = system()

    Observabilite = obsv(sys.A,sys.C)
    print("Le déterminant de la matrice observabilité est:",np.linalg.det(Observabilite),". Il est différent de 0 donc le système est observable.")

    L = OKT(sys)
# %%
