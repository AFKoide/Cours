import numpy as np
import matplotlib.pyplot as plt
from math import *

l1 = 0.6; l2 = 0.4

def MGD(theta1,theta2):
    theta1 = theta1*pi/180 ; theta2 = theta2*pi/180
    x1 = l1*cos(theta1) ; y1 = l1*sin(theta1)
    x2 = l2*cos(theta1 + theta2) ; y2 = l2*sin(theta1 + theta2)
    x = x1 + x2; y = y1 + y2

    x_bras = [0,x1,x]; y_bras = [0,y1,y];

    plt.plot(x_bras,y_bras,x,y,'+');plt.title("Modèle Géométrique Direct")
    plt.gca().axis('equal')
    return x, y


def MGI():

    return None




MGD(24,17)
