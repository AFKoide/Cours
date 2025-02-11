#%% CURTIS MARTELET

# Press ⌃R to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.


# TP 3 Observabilité et synthèse d'observateur
# partie 2 : synthèse d'observateur pour des systèmes linéaiers continus


# fonctin qui simule un système continu (à ne pas toucher)
import numpy as np
import os
from control.matlab import *
import matplotlib.pyplot as plt  # MATLAB plotting functions
def system(t, u):
    # Définition des matrices d'état
    A = [[0., 0., 1.], [1., -1., 2.], [-6., 0., -5.]]
    B = [[1.], [2.], [1.]]
    C = [1., 1., 1.]
    D = [0.]
    # vecteur d'état initial
    x0 = [[4.0], [-3.0], [1.0]]

    # Calcul des valeurs propres et de la matrice de passage
    valPropres, T = np.linalg.eig(A)

    # les valeurs propres de A
    lambda1 = valPropres[0]
    lambda2 = valPropres[1]
    lambda3 = valPropres[2]

    # La matrice diagonalisée de A
    D = [[lambda1, 0., 0.], [0., lambda2, 0.], [0., 0., lambda3]]

    # Calcul de la matrice exponentiel e(At)
    exp_Dt = [[np.exp(lambda1*t), 0., 0.],
              [0., np.exp(lambda2*t), 0.], [0., 0., np.exp(lambda3*t)]]

    # Ce calcul est expliqué dans le cours (slide 30)
    exp_At = T@exp_Dt@np.linalg.inv(T)

    # calcul de la solution de l'équation différentielle (slide 26)

    # calcul de la solution homogène (slide 25)
    xh = exp_At @ x0

    # calcul de la solution particulière (slide 26)
    xp = -np.linalg.inv(A)@B + exp_At @ np.linalg.inv(A) @ B

    # solution complète de l'équation d état
    x = xh + xp

    # calcul de la sortie du système
    y = C @ x

    return y

##############################################################################
# La fonction observateur à mettre ici


def Observateur(u,y,x_hat):
    Te = 0.001
    A = np.array([[0, 0, 1], [1, -1, 2], [-6, 0, -5]])
    B = np.array([[1], [2], [1]])
    C = np.array([[1, 1, 1]])
    D = 0

    sys = ss(A, B, C, D)
    sysd = c2d(sys,Te)
    Phi = sysd.A
    Gamma = sysd.B

    L = np.array([[-6.], [9.], [6.]])

    x_cha = Phi @ x_hat + Gamma*u + (L*Te) @ (y - (C @ x_hat))
    return x_cha

if __name__ == '__main__':
    C = np.array([[1, 1, 1]])
    # On simule le système pendant 10 s (= N*Te)
    # N est le nombre d'échantillon
    N = 10000

    # définiton de la periode d'échantillonage
    Te = 0.001

    # vecteurs et matrices de sauvegarde
    t = np.zeros(N)
    
    yy = np.zeros(N)
    yy_hat = np.zeros(N)
    
    xx_hat = np.array([[2],[0],[0]])
    # vecteur d'état estimée à l'instant k = 0
    x_hat = xx_hat

    # entrée echelon
    u = 1.0
    for i in range(0, N):
        y = system(Te*i, u)
        yy[i] = y
        t[i] = Te*i
        if i>= 1:
            x_hat = Observateur(u,y,x_hat)
            xx_hat = np.hstack((xx_hat,x_hat))
        yy_hat[i] = C @ x_hat
    

    plt.figure(1)
    plt.plot(t, yy, t, yy_hat)
    plt.legend(['y(t)', 'y(t)_estimé']);plt.grid('minor')
    plt.show()
    "On peut observer que l'ajout d'un observateur n'a pas beaucoup changé le tracé de la réponse."

    plt.figure(2)
    plt.subplot(3,1,1)
    plt.plot(t,xx_hat[0,:].T);plt.grid('minor');plt.legend(["X^[0]"])
    plt.subplot(3,1,2)
    plt.plot(t,xx_hat[1,:].T);plt.grid('minor');plt.legend(["X^[1]"])
    plt.subplot(3,1,3)
    plt.plot(t,xx_hat[2,:].T);plt.grid('minor');plt.legend(["X^[2]"])
    plt.show()
# %%
