# -*- coding: utf-8 -*-
"""
Created on Wed Oct 11 16:09:32 2023

@author: simu05
"""

import numpy as np
import matplotlib.pyplot as plt
from math import sqrt, pi

a=2 ; b=-3
x=np.random.randn(1,200)*10
y= a*x + b + np.random.randn(np.size(x))*5

plt.figure()
plt.plot(x,y,'.');plt.axis('equal');plt.show()


X = np.hstack((x.T,y.T))

y0 = np.mean(y)
x0 = np.mean(x)
plt.plot(x0,y0,'+');plt.axis('equal')


M = (1/len(x)) * X.T @ X

ValP,VecP = np.linalg.eig(M)

YT = X @ VecP
Y = YT.T
plt.figure(2)
plt.plot(Y[:, 0],Y[:,1],'.')


t = np.linspace(-pi,pi, 100)
plt.plot( x0+2*sqrt(ValP[0])*np.cos(t) , y0+3*sqrt(ValP[1])*np.sin(t) )
plt.grid(color='lightgray',linestyle='--')
# plt.legend(["Valeurs Propres","Ellipse"])


#%% Fonction
a=2 ; b=-3
x=np.random.randn(1,200)*10
Nuage = a*x + b + np.random.randn(np.size(x))*5

def AnalyseNuageDePoint(Nuage):
    barycentre = [np.mean(Nuage[:,0]) , np.mean(Nuage[:,1])]
    M = (1/len(Nuage[:,1])) * Nuage.T @ Nuage
    
    
    Y = (X @ VecP).T
    allongement = 3*np.sqrt([Y[:, 0],Y[:, 1]])
    
    return barycentre, allongement




[barycentre, allongement] = AnalyseNuageDePoint(Nuage)
