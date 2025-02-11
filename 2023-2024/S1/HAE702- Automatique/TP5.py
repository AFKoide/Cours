#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Dec  7 12:25:19 2023

@author: curtis.martelet@etu.umontpellier.fr
"""

import numpy as np
import matplotlib.pyplot as plt
from math import *

l1 = 0.6; l2 = 0.4




def MGD(theta1,theta2):
    x1 = l1*cos(theta1)
    y1 = l1*sin(theta1)
    x2 = l2*cos(theta1 + theta2)
    y2 = l2*sin(theta1 + theta2)
    x = x1 + x2; y = y1 + y2
    x_bras = [0,x1,x]; y_bras = [0,y1,y]
    x_bras = [0,x1,x]; y_bras = [0,y1,y]
    plt.plot(x_bras,y_bras,'b',x,y,'r+')
    plt.gca().axis('equal')





if __name__ == "__main__":
# Partie 1
    plt.figure()
    dT = 10e-3; vitesse = 1
    

    theta1 = np.zeros(101); theta2 = np.zeros(101)
    theta1[0] = 17*pi/180; theta2[0] = 90*pi/180
    for i in range(1,100+1):
        MGD(theta1[i-1],theta2[i-1])
        theta1[i] = theta1[i-1] + vitesse*dT
        theta2[i] = theta2[i-1] + vitesse*dT
    plt.title("Modèle Géométrique direct (BO)"); plt.grid()
    plt.show()



# Partie 2
    plt.figure()
    J = np.zeros((2,2)); vitesse = np.array([[-0.1],[0]]) 

    q = np.zeros((2,1))
    q[0] = 0.2; q[1] = 0.6     # Angle d'origine
    for i in range(1,100+1):
        theta1 = q[0]
        theta2 = q[1]
        J = np.array([[-l1*sin(theta1)-l2*sin(theta1+theta2),-l2*sin(theta1+theta2)],
                      [l1*cos(theta1)+l2*cos(theta1+theta2),l2*cos(theta1+theta2)]])
        # a, b, c, d = J.flatten()
        # determinant = a * d - b * c
        # J1 = np.array([[d, -b], [-c, a]]) / (a * d - b * c)
        J1 = np.linalg.inv(J.T@J+1e-5*np.identity(2))@J.T

        q = (q + np.dot(J1,vitesse)*dT).reshape((2,1))
        
        MGD(theta1,theta2)
    plt.title("Modèle Géométrique direct (BF)"); plt.grid()
    plt.show()



# Partie 3
    x_desire = 0.7; y_desire = 0.3
    x_init = 0 ; y_init = 1
    l = sqrt(x_desire**2 + y_desire**2)

    theta2 = pi - acos((l1**2+l2**2-l**2) / (2*l2*l1))
    theta1 = atan2(y_desire,x_desire) - acos((l1**2+l**2-l2**2) / (2*l*l1))
    
    print(f"Theta1 = {np.degrees(theta1)} deg, Theta2 = {np.degrees(theta2)}")
    MGD(theta1,theta2)
    plt.title("Modèle Géométrique Inverse (calcul direct)"); plt.grid()
    plt.show()

    theta1 = np.zeros(101); theta2 = np.zeros(101)
    for i in range(1,100+1):
        if i == 1:
            x_actuel = x_init
            y_actuel = y_init
        else:
            x_actuel = l2*cos(theta1[i-1] + theta2[i-1]) + l1*cos(theta1[i-1])
            y_actuel = l1*sin(theta1[i-1]) + l2*sin(theta1[i-1] + theta2[i-1])

        x_erreur = x_desire - x_actuel
        y_erreur = y_desire - y_actuel
        l = sqrt(x_actuel**2 + y_actuel**2)

        theta1[i] = theta1[i-1] + acos((-l1**2-l2**2+l**2) / (2*l2*l1)) * dT
        theta2[i] = theta2[i-1] + (atan2(y_erreur,x_erreur) - acos((l1**2+l**2-l2**2) / (2*l*l1))) * dT
        print(f"Iteration {i}: Erreur x = {x_erreur}, Erreur y = {y_erreur}, Theta1 = {(theta1[i])} rad, Theta2 = {(theta2[i])} rad")
        MGD(theta1[i],theta2[i])
    plt.title("Modèle Géométrique Inverse"); plt.grid()
    plt.show()