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



def calculateAndDrawRobotPose(v, w, xPrev, yPrev, thetaPrev, col) :       
    dT = 0.05 #seconds
    # Discrétisation de Intégration
    # xCur = xPrev
    # yCur = yPrev
    # thetaCur = thetaPrev
    thetaCur = thetaPrev + w*dT
    xCur = xPrev + v*np.cos(thetaPrev)*dT # (cos(thetaCur) - cos(thetaPrev)) # (cos(thetaPrev) - cos(thetaPrev))
    yCur = yPrev + v*np.sin(thetaPrev)*dT # (sin(thetaCur) - sin(thetaPrev)) # (sin(thetaPrev) - sin(thetaPrev))

    #laisser les lignes ci-dessous
    if i % 50 == 0 :
        drawRobot(xCur, yCur, thetaCur, col)
    plt.plot(xCur, yCur, marker='o', markersize=1, color=col)
    time = i*dT
    print("i is ", i, "time is %.2f" %time, " x %.3f" %xCur, " y %.3f" %yCur, "theta %.3f" %thetaCur)    
    return (xCur, yCur, thetaCur)    






if __name__ =='__main__':
    plt.figure(0)
    N = 1000 #iterations
    x = np.zeros(N+1)
    y = np.zeros(N+1)
    theta = np.zeros(N+1)
    vel = np.zeros(N+1)
    omega = np.zeros(N+1)
    
    #control to desired position
    #initial pose
    x[0] = 0
    y[0] = 0 
    theta[0] = -3.1415
    drawRobot(x[0], y[0], theta[0], 'blue')    
    #desired position
    x_des = 100
    y_des = -4
    #loop
    for i in range (1,N+1):
        vel[i] = np.sqrt((x_des-x[i-1])**2 + (y_des-y[i-1])**2)
        omega[i] = normalizeAngle(atan2(y_des - y[i-1], x_des - x[i-1]) - theta[i-1])
        
        x[i], y[i], theta[i] = calculateAndDrawRobotPose(vel[i], omega[i], x[i-1], y[i-1], theta[i-1], 'lightsteelblue')
        if (vel[i] <= 0.001):
            print(vel[i])
            break
        
    #draw final robot pose
    drawRobot(x[i], y[i], theta[i], 'midnightblue')
    plt.plot(x_des, y_des, 'ko')
    
    plt.title("Le robot doit aller là")
    plt.legend(["Position Init Robot","Trajectoire du robot","Position Finale Robot"])
    plt.show()
    
    
    
    
    
    plt.figure(1)
    N = 1000 #iterations
    x = np.zeros(N+1)
    y = np.zeros(N+1)
    theta = np.zeros(N+1)
    vel = np.zeros(N+1)
    omega = np.zeros(N+1)
    
    #follow a path
    #initial pose
    x[0] = 4
    y[0] = -1
    theta[0] = 0
    drawRobot(x[0], y[0], theta[0], 'green')

    xLine = np.linspace(-5,5,100) ; 
    yLine = 0*xLine
    
    Kd = -1 ; Ka = -1
    
    #desired path ax+by+c=0
    a=7 ; b=6 ; c=5 ; beta = -atan2(a,b)
    #trace la trajectoire voulue
    x_courbe = np.linspace(-5,5,100);
    y_courbe = -(a*x_courbe + c)/b
    plt.plot(x_courbe,y_courbe);
    
    #loop
    for i in range (1,N+1):
        #TODO feedback control
        vel[i] = 1
        
        # Suivit de la ligne noire
        # d = y[i-1]
        # alpha = normalizeAngle(theta[i-1])
        # omega[i] = Kd*d + Ka*alpha
        
        # Suivit d'une trajectoire donnée
        d = (a*x[i-1]+b*y[i-1]+c)/(sqrt(a**2+b**2))
        alpha = normalizeAngle(theta[i-1]-beta) # alpha = normalizeAngle(beta-theta[i-1])
        omega[i] = Kd*d + Ka*alpha
    
        
        x[i], y[i], theta[i] = calculateAndDrawRobotPose(vel[i], omega[i], x[i-1], y[i-1], theta[i-1], 'springgreen')
    #draw final robot pose
    drawRobot(x[i], y[i], theta[i], 'darkgreen')
    #draw desired path
    plt.plot(xLine, yLine, '-k')
    #show plot 
    plt.title("Le robot doit suivre ça")
    plt.legend(["Robot","Trajectoire voulue","Trajectoire du robot"])
    plt.show()  
