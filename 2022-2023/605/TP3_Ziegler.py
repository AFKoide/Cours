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

print("\nMéthode de Ziegler")
plt.figure()
y,T= step(feedback(H,1))
plt.plot(T,y)

K0=2.7; T0=1.16;
# P
Krp = 0.5*K0
Cp=tf(Krp,1)
yp,Tp= step(feedback(Cp*H,1))
print("P\nC= ",Cp,"Feedback= ",feedback(Cp*H,1))
plt.plot(Tp,yp)
# PI
Krpi = 0.45*K0; τipi = 0.83*T0
Cpi = tf([Krpi*τipi,Krpi],[τipi])
ypi,Tpi= step(feedback(Cpi*H,1))
print("PI\nC= ",Cpi,"Feedback= ",feedback(C*H,1))
plt.plot(Tpi,ypi)
# PID
Krpid = 0.6*K0; τipid = 0.5*T0; τppid = 0.125*T0
Cpid = tf([Krpid*τppid*τipid,Krpid*τipid,Krpid],[τipid])
ypid,Tpid= step(feedback(Cpid*H,1))
print("PID\nC= ",Cpid,"Feedback= ",feedback(Cpid*H,1))
plt.plot(Tpid,ypid)
plt.title("Réponse Indicielle selon le PID avec la méthode de Ziegler")
plt.xlabel('temps (s)');plt.ylabel('Réponse')
plt.legend(["Rien","P","PI","PID"])
plt.grid(True);plt.show()