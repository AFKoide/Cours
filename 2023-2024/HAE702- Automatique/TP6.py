#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Dec  7 00:07:41 2020

@author: salihabdelaziz
"""

# This is a sample Python script.

# Press ⌃R to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.
import numpy as np
import os
from control.matlab import *
import matplotlib.pyplot as plt  # MATLAB plotting functions
from math import *
import math
from scipy import integrate
import scipy as sc

# constantes du programme
M = 3.2         # Masse du chariot
m = 0.329       # Masse du pendule
J = 0.072       # Iniertie du pendule
l = 0.44        # Longueur du pendule
g = 9.81
fr = 6.2        # Frottement fluide du chariot
fphi = 0.009      # Frottement fluide du pendule
Kf = 2.6        # Coefficient de force [N/V]
n1 = 14.9       # Capteur de position [V/m]
n2 = -52.27     # Capteur d'angle [V/rd]
n3 = -7.64      # Capteur de vitesse [V/m/s]
n4 = -52.27     # Capteur de vitesse angulaire [V/rd/s]
Ks= Kf*1.;        # Coefficient de friction in N
gs= -44.29


# définition des matrices d'état du système linéarisé
# définition de la matrice A
A1 = np.array([[0., 0., 1., 0.], [0., 0., 0., 1.]])
tmp1 = np.array([[M+m, m*l], [m*l, J+m*l*l]])
tmp2 = np.array([[0., 0., -fr, 0.], [0., m*g*l, 0., -fphi]])
A2 = np.linalg.inv(tmp1)@tmp2
A = np.block([[A1], [A2]])
#print("A vaut\n", A)

# définition de la matrice B
B1 = np.array([[0.], [0.]])
tmp3 = np.array([[Kf], [0]])
B2 = np.linalg.inv(tmp1)@tmp3
B = np.block([[B1], [B2]])


# Définitionde la matrice C
C = np.array([[n1, 0, 0, 0], [0, n2, 0, 0], [0, 0, n3, 0]])

# Définition de la matrice D
D = np.array([[0], [0],[0]])

    
# Définition du temps de simulation 
N = 10000

# définiton de la periode d'échantillonage
Te = 0.001


# définition du vecteur d'état initial de la fonction pendule_MNL x = [x1; x2; x3; x4]
x = np.zeros((4, 1))

# définition des matrices nécessaires pour mémoriser le vecteur d'état x dans la matrice xx
# et la dérivée du vecteur d'état x, nommée dx, dans la matrice dxx

dxx = np.zeros((4, N+1))
xx = np.zeros((4, N))       
yy = np.zeros((3, N))       # pour stokker la sortie
t = np.zeros(N)             # pour stocker le temps 

dxx[0, 0] = x[0, 0]
dxx[1, 0] = x[1, 0]
dxx[2, 0] = x[2, 0]
dxx[3, 0] = x[3, 0]
  
# initialisation du vecteur d'état estimé et de la commande
x_hat = np.zeros((5, 1))
u = 0.
xx_hat = np.zeros((5, N))


##############################################################################
#               A ne pas toucher
##############################################################################
def pendule_MNL(u, i):
    r       = x[0, 0]
    phi     = x[1, 0]
    dr      = x[2, 0]
    dphi    = x[3, 0]
    
    A1=np.array([[M+m, m*l*cos(phi)], [m*l*cos(phi), J+m*l*l]])
    
    f1 = -fr*dr + m*l*dphi*dphi*sin(phi) + Kf*u - Ks*np.sign(dr);
    f2 = m*g*l*sin(phi) - fphi*dphi;

    dxx1=np.array([[dr], [dphi]])
    dxx2=np.linalg.inv(A1)@np.array([[f1], [f2]])
    dx=np.block([[dxx1], [dxx2]])
    
    dxx[0, i+1]=dx[0]
    dxx[1, i+1]=dx[1]
    dxx[2, i+1]=dx[2]
    dxx[3, i+1]=dx[3]
    
    x[0, 0] = np.trapz([dxx[0, i], dxx[0, i+1]], dx=Te) + x[0, 0]
    x[1, 0] = np.trapz([dxx[1, i], dxx[1, i+1]], dx=Te) + x[1, 0]
    x[2, 0] = np.trapz([dxx[2, i], dxx[2, i+1]], dx=Te) + x[2, 0]
    x[3, 0] = np.trapz([dxx[3, i], dxx[3, i+1]], dx=Te) + x[3, 0]
    
    y = C@x
    
    return y

##############################################################################
#               A mettre ici votre observateur de Kalamn discret
##############################################################################
# Question 7 : 

# def observateur(u, y, x_hat):
     
    


# Press the green button in the gutter to run the script.
if __name__ == '__main__':  
    
    
    # Question 1 : Analyse de stabilité
    sys_ss = ss(A,B,C,D)
    print(sys_ss)
    y,T = step(sys_ss)
    plt.figure();plt.xlabel("Le système est instable.")
    plt.plot(T,y);plt.grid('on');plt.title("Evolution du pendule")
    plt.show()
    
    
    # Question 2 : Analyse de commandabilité du système linéairisé
    commandabilite = ctrb(A,B)
    print("La commandabilité est:",np.linalg.det(commandabilite)," : le système est donc commandable car != 0")
    
    
    # Question 2 : Analyse d'observabilité
    observabilite = obsv(A,C)
    print("Le rang de la matrice observabilité est:",np.linalg.matrix_rank(observabilite),". Le système est donc observable.")
    
    # Question 3 
    # définition de la représentation d'état du système augmenté
    
    
      
    
    #Question 4
    # utiliser plutôt la fonction cont2discrete de scipy
    
    
    # Question 5 : Calcul des poles de l'observateur de Kalamn discret
    
    
    # Question 6 Calcul du gain de Kalman Ld
    
    
      
    
    # Question 8 : # placement des poles du retour d'état et calcul de K
    
    
    

    
    for i in range(0, N) :
        
        # Appel du système non linéaire
        y = pendule_MNL(u, i)

        # Question 7 : Appel de l'observateur
       
        
        
        # Question 9 :  Calcul de la commande
        

        
        ###################################################
        # Sauvegarde des données
        xx_hat[0, i]=x_hat[0]
        xx_hat[1, i]=x_hat[1]
        xx_hat[2, i]=x_hat[2]
        xx_hat[3, i]=x_hat[3]
        xx_hat[4, i]=x_hat[4]
        
        yy[0, i] = y[0]
        yy[1, i] = y[1]
        yy[2, i] = y[2]
        
        t[i]=Te*i
        
    
    
    
    plt.figure(1)    
    plt.plot(t, yy[0, :])
    plt.xlabel('Time (s)')
    plt.ylabel('Amplitude(rad/s)')
    plt.legend(['y1(t)'])
    plt.title('position du chariot')
    plt.grid()
    
    plt.figure(2)    
    plt.plot(t, yy[1, :])
    plt.xlabel('Time (s)')
    plt.ylabel('Amplitude(rad/s)')
    plt.legend(['y2(t)'])
    plt.title('position angulaire du pendule')
    plt.grid()
    
    plt.figure(3)    
    plt.plot(t, yy[2, :])
    plt.xlabel('Time (s)')
    plt.ylabel('Amplitude(rad/s)')
    plt.legend(['y3(t)'])
    plt.title('vitesse de déplacement du chariot')
    plt.grid()
    
    
    
    


