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
    x1 = l1*cos(theta1) ;
    y1 = l1*sin(theta1)
    x2 = l2*cos(theta1 + theta2) ;
    y2 = l2*sin(theta1 + theta2)
    x = x1 + x2; y = y1 + y2

    x_bras = [0,x1,x]; y_bras = [0,y1,y];

    x_bras = [0,x1,x]; y_bras = [0,y1,y];
    plt.plot(x_bras,y_bras,'b',x,y,'r+');plt.title("Modèle Géométrique Direct")
    plt.gca().axis('equal')


def MGI():

    return None




if __name__ == "__main__":
    dT = 10e-3; vitesse = 10
    
    # theta1 = np.zeros(100); theta2 = np.zeros(100)
    # x = np.zeros(100); y = np.zeros(100); x1 = np.zeros(100); y1 = np.zeros(100)

    # x[0] = 0; y[0] = pi
    # theta1[0] = 0*pi/180; theta2[0] = 0*pi/180
    # for i in range(1,100):
    #     x, y, x1, y1 = MGD(theta1[i-1],theta2[i-1])
    #     theta1[i] = theta1[i-1] + vitesse*dT
    #     theta2[i] = theta2[i-1] + vitesse*dT
    # plt.show()
    

    J = np.zeros((2,2)); vitesse = np.array([[0.1],[0]])

    q = np.zeros((2,1))
   
    q[0] = 0.2; q[1] = 0.6
    for i in range(1,100):
        theta1 = q[0];
        theta2 = q[1]
        J = np.array([[-l1*sin(theta1)-l2*sin(theta1+theta2),-l2*sin(theta1+theta2)],
         [l1*cos(theta1)+l2*cos(theta1+theta2),l2*cos(theta1+theta2)]])
        J1 = np.linalg.inv(J)

        tmp1 = q
        q = (q + np.dot(J1,vitesse)*dT).reshape((2,1))
        
        
        MGD(theta1,theta2)