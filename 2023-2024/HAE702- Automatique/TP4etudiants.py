#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Dec 10 22:24:16 2020

@author: andrea
"""

import numpy as np
import matplotlib.pyplot as plt
from math import *

def normalizeAngle(angle):
    while angle >= np.pi:
        angle -= 2.0 * np.pi
    while angle < -np.pi:
        angle += 2.0 * np.pi
    return angle 

def drawRobot (x, y, t, color) :
     l=0.5 #robot size
     point1 = [x + l*sin(t), y - l*cos(t)]
     point2 = [x + 4*l*cos(t), y + 4*l*sin(t)]
     point3 = [x - l*sin(t), y + l*cos(t)]
     x_values = [point1[0], point2[0], point3[0], point1[0]]
     y_values = [point1[1], point2[1], point3[1], point1[1]]
     plt.plot(x_values, y_values, color)
     
def calculateAndDrawRobotPose(v, w, xPrev, yPrev, thPrev, col) :       
    dT = 0.05 #seconds
    #TODO changer ici
    xCur = xPrev
    yCur = yPrev
    thCur = thPrev
    #laisser les lignes ci-dessous
    if i % 50 == 0 :
        drawRobot(xCur, yCur, thCur, col)
    plt.plot(xCur, yCur, marker='o', markersize=1, color=col)
    time = i*dT
    print("i is ", i, "time is %.2f" %time, " x %.3f" %xCur, " y %.3f" %yCur, "th %.3f" %thCur)    
    return (xCur, yCur, thCur)    

if __name__ =='__main__':
    N = 1000 #iterations
    x = np.zeros(N+1)
    y = np.zeros(N+1)
    th = np.zeros(N+1)
    vel = np.zeros(N+1)
    omega = np.zeros(N+1)
    
    #control to desired position
    #initial pose
    x[0] = 4 
    y[0] = -2 
    th[0] = -3.1415
    drawRobot(x[0], y[0], th[0], 'blue')    
    #desired position
    x_des = -4
    y_des = 4
    #loop
    for i in range (1,N+1):
        #TODO feedback control
        vel[i] = 0
        omega[i] = 0
        x[i], y[i], th[i] = calculateAndDrawRobotPose(vel[i], omega[i], x[i-1], y[i-1], th[i-1], 'lightsteelblue')
    #draw final robot pose
    drawRobot(x[i], y[i], th[i], 'midnightblue')

    #follow a path
    #initial pose
    x[0] = -1
    y[0] = -2 
    th[0] = 0
    drawRobot(x[0], y[0], th[0], 'green')
    #desired path ax+by+c=0
    xLine = np.linspace(-5,5,100)
    yLine = 0*xLine
    #loop
    for i in range (1,N+1):
        #TODO feedback control
        vel[i] = 0
        omega[i] = 0
        x[i], y[i], th[i] = calculateAndDrawRobotPose(vel[i], omega[i], x[i-1], y[i-1], th[i-1], 'springgreen')
    #draw final robot pose
    drawRobot(x[i], y[i], th[i], 'darkgreen')
    #draw desired position and path
    plt.plot(x_des, y_des, 'ko')
    plt.plot(xLine, yLine, '-k')
    #show plot
    plt.xlim(-5, 5)
    plt.ylim(-5, 5)    
    plt.show()
    plt .figure()    