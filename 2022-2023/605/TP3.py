import numpy as np
import matplotlib.pyplot as plt
from control.matlab import *
from scipy import signal

plt.close('all')

### PARTIE 1
print("\n\n\nPARTIE 1")
num = 100
den = [1,10,29,20]

print("Fonction de Transfert")
H = tf(num,den)
print(H)

z,p,k = tf2zpk(num,den)
print("Zéros: ",z,"\tPôles: ",p,"\tGain: ",k)

#Determination de la réponse impulsionnelle
print("\n\nRéponse Impulsionnelle")
Im_T,Im_yout = signal.impulse((num,den))

plt.figure(num="Impulsionnelle")
plt.plot(Im_T,Im_yout)
plt.title("Réponse Impulsionnelle")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.legend(["Impulsionnelle","Tangente"])
plt.grid(True);plt.show()

print("Le point d'inflection est: ",Im_yout.max())

#Determination de la réponse indicielle
print("\n\nRéponse Indicielle")
In_yout,In_T = step(H,7)

plt.figure(num="Indicielle")
plt.plot(In_T,In_yout)

# Tangente
t = np.arange(0.1,2100)*0.001
inflection_x=Im_T[np.argmax(Im_yout)]
inflection_y=Im_yout.max()
plt.axvline(inflection_x,color='g',linestyle='-.') #On trouve f(a) avec le croisement entre la courbe verte et la courbe bleue

tangente=inflection_y*(t-inflection_x)+1.27
plt.plot(t,tangente)
plt.axhline(In_yout[-1],color='k',linestyle=':')
plt.axhline(In_yout[0],color='k',linestyle=':')
plt.title("Réponse Indicielle")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.legend(["Indicielle","f(a)","Tangente"])
plt.grid(True);plt.show()

# STREJC
print("\nSTREJC")
T2 = 1.98;T1 = 0.26
Ta=T2-T1; Tu=T1; dy=In_yout[-1]-In_yout[0]
rapport = Tu/Ta;n = 2;τ = Ta/n;nTu=Ta/0.1036;r = nTu-Tu

print("n: ",n,"\nτ: ",τ,"\nr: ",r)


## Méthode de Broida
print("\nMéthode de Broida")
plt.figure(num="Broida")
plt.plot(In_T,In_yout)
plt.axhline(In_yout[-1]*0.28,color='k',linestyle=':')
plt.axhline(In_yout[-1]*0.4,color='k',linestyle=':')
plt.title("Réponse Indicielle selon Broida")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.legend(["Indicielle"])
plt.grid(True);plt.show()

t1=0.759;t2=0.97

τ = 5.5*(t2-t1)
r = 2.8*t1-1.8*t2
print("nτ: ",τ,"\nr: ",r)


### PARTIE 2 : PID
print("\n\n\nPARTIE 2")
plt.figure(num="PDI")
y,T= step(feedback(H,1),12)
# y,T= step(H,12)
plt.plot(T,y)

K = In_yout[-1];
# P
# Le filtre P est plus rapide que le signal sans correcteur.
# Cependant, ce filtre n'est pas suffisant car on a toujours une erreur de position.
# On peut donc stipuler que ce correcteur sert à rendre la correction plus rapide.
Kr= Ta/(K*Tu)
Cp= tf(Kr,1)
y,T= step(feedback(Cp*H,1),12)
print("P\nC= ",Cp,"Feedback= ",feedback(Cp*H,1))
plt.plot(T,y)

# PI
# Le filtre PI (correcteur P+I), quand à lui, corrige complètement la valeur finale : l'erreur statique est nulle.
# Cependant, le signal oscille beaucoup trop.
# On peut stipuler que le correcteur I sert à corriger la valeur finale.
Kr= 0.9*Ta/(K*Tu); τi = 3.3*Tu
Cpi= tf([Kr*τi,Kr],[τi,0])
y,T= step(feedback(Cpi*H,1),12)
print("PI\nC= ",Cpi,"Feedback= ",feedback(Cpi*H,1))
plt.plot(T,y)

# PID
# Le filtre PID corrige complètement le signal : le temps de réponse est rapide, on ne relève aucune erreur statique et le signal de sortie n'oscille plus rapide.
# On peut stipuler que le correcteur D réduit significativement l'oscillation du signal de sortie.
Kr= 1.2*Ta/(K*Tu); τi = 2*Tu; τp = 0.5*Tu
Cpid= tf([Kr*τp*τi,Kr*τi,Kr],[τi,0])
y,T= step(feedback(Cpid*H,1),12)
print("PID\nC= ",Cpid,"Feedback= ",feedback(Cpid*H,1))
plt.plot(T,y)

plt.title("Réponse Indicielle selon le PID")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.legend(["Rien","P","PI","PID"])
plt.grid(True);plt.show()


# Méthode de Ziegler
print("\nMéthode de Ziegler")
plt.figure(num="Ziegler")
y,T= step(feedback(H,1),12)
# y,T= step(H,12)
plt.plot(T,y)

K0=2.7; T0=1.16;
# PID
# On peut observer avec la courbe que le filtre PID déterminé en boucle fermé est quasiment identique à celle déterminée par la méthode en boucle ouverte.
Kr = 0.6*K0; τi = 0.5*T0; τp = 0.125*T0
C = tf([Kr*τp*τi,Kr*τi,Kr],[τi,0])
y,T= step(feedback(C*H,1),12)
print("PID\nC= ",C,"Feedback= ",feedback(C*H,1))

# P
Kr = 0.5*K0
C=tf(Kr,1)
y,T= step(feedback(C*H,1),12)
print("P\nC= ",C,"Feedback= ",feedback(C*H,1))
plt.plot(T,y)

# PI
Kr = 0.45*K0; τi = 0.83*T0
C = tf([Kr*τi,Kr],[τi,0])
y,T= step(feedback(C*H,1),12)
print("PI\nC= ",C,"Feedback= ",feedback(C*H,1))
plt.plot(T,y)

plt.plot(T,y)
plt.title("Réponse Indicielle selon le PID avec la méthode de Ziegler")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.legend(["Rien","PID","P","PI"])
plt.grid(True);plt.show()