#%% Random
import numpy as np
import matplotlib.pyplot as plt
from random import random

t = np.arange(0,1,0.1)
x = random()
plt.figure()
plt.plot(x,x,'+');plt.axis([0,1,0,1]);plt.show()

t = np.arange(0,1,0.1)
x = random()
plt.figure(figsize=(1,1))
plt.plot(x,x,'+');plt.axis([0,1,0,1]);plt.show()
x = random()
plt.figure(figsize=(0.25,0.25))
plt.plot(x,x,'+');plt.axis([0,1,0,1]);plt.show()
# La commande figsize permet de contrôler la taille des graphiques (en inch) dans une fenêtre. 
# Regardez l’aide de cette fonction. Essayez de dessiner la variable x dans une autre figure qui n’occuperait que le quart de la fenêtre et positionnée en haut à gauche.

#%%
import numpy as np
import matplotlib.pyplot as plt

t = np.arange(0,10,0.1)
x = np.sin(np.pi*t) ; y=np.sinc(np.pi*t) ; z=np.cos(np.pi*t)

plt.plot(t,x,t,y,t,z);plt.xlabel("Temps");plt.ylabel("Valeur");plt.legend(["x","y","z"])
plt.show()

plt.figure()
plt.subplot(3,1,1);plt.plot(t,x);plt.xlabel("Temps");plt.ylabel("Valeur");
plt.subplot(3,1,2);plt.plot(t,y);plt.xlabel("Temps");plt.ylabel("Valeur");
plt.subplot(3,1,3);plt.plot(t,z);plt.xlabel("Temps");plt.ylabel("Valeur");

#%% Handle
import numpy as np
import matplotlib.pyplot as plt

t = np.arange(0,1,0.1)
x = random()
fig1 = plt.figure(1)
plt.plot(x,x,'+');plt.axis([0,1,0,1]);plt.show()

h = plt.getp(fig1)

plt.setp(fig1,visible='0')
plt.setp(fig1, visible='1')

#%% Animation
import numpy as np
import matplotlib.pyplot as plt

x = np.arange(-3,3,0.1)
y = x
X, Y = np.meshgrid(x, y)
for a in range(30): 
    z = np.sinc(np.sin(a*np.pi/60)*X)*np.sinc(np.sin(a*np.pi/60)*Y)
    fig1=plt.figure(1)
    ax = plt.axes(projection='3d')
    ax.plot_surface(X, Y, z, cmap='viridis')
    plt.show()



#%% PARTIE 4 : VARIABLE
# from dill import dump_session

# dump_session("tp3")



#%% PARTIE 5 : ALGEBRE
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
