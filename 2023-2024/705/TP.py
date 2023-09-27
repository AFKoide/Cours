import math as m
import numpy as np
import matplotlib.pyplot as plt
import random as rand
from math import *
from numpy.linalg import inv
from scipy import stats

# x = [1,2,3]
# y = np.array([[1],[2],[3],[4]])

# z = 2*(512*m.cos(pi/3)+13**2)/127

# A = np.array([[1,2,3],[4,5,6]])
# B = np.array([[1,2],[3,4],[5,6]])
# C = np.dot(A,B)
# D = np.dot(B,A)
# # E = B + A # Ce calcul ne fonctionne pas car pas la meme dimension
# E = A + A

# F = np.array([[1,2],[3,4]])
# G = np.array([[5,6],[7,8]])
# H = F @ G
# I = F * G
# # L'opérateur * réalise une multiplication terme par terme tandis que @ réalise un produit matriciel

# J = A.T


#--------------------------------------

# X = np.linspace(0,10,50)
# a = 2; b = -3
# Y = a*X+b

# plt.figure(10)
# plt.plot(X,Y,'r-')

# Y = Y + (np.random.randn(np.size(Y))*np.sqrt(2))
# plt.figure(10)
# plt.plot(X,Y,'b.')

# B = Y.reshape(50,1)
# A = np.hstack((X.reshape(50,1),np.ones((50,1))))

# C = inv(A.T @ A) @ A.T @ B
# Yc = C[0]*X + C[1] - Y
# plt.plot(X,Yc,'k--')

# Erreur = sum(Yc**2)/len(Yc)
# print("L'erreur est:", Erreur)


#--------------------------------------

# from mpl_toolkits.mplot3d import axes3d
# x = np.arange(-4,4,0.2) ; y = x
# X, Y = np.meshgrid(x, y)
# z = np.exp(-X**2)*np.exp(-Y**2)
# fig7=plt.figure(7)
# ax = plt.axes(projection='3d') ; ax.plot_surface(X, Y, z, cmap='viridis')
# fig8=plt.figure(8)
# ax = plt.axes(projection='3d') ; ax.plot_wireframe(X, Y, z, cmap='viridis')


#--------------------------------------
x = np.linspace(-1,1,50)

a = np.zeros(len(x))
while i in range(len(x)):
    a[i] = acos(x[i])
    
plt.plot(x,a)
