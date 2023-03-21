from math import acos,cos,sin,atan2,sqrt
import numpy as np
import matplotlib.pyplot as plt


## CALCUL DE L1 L2 L0-L3 
q11= np.radians(90) ;q21= np.radians(51) ;q31= np.radians(42) ;q41= np.radians(90)
q12= np.radians(0)  ;q22= np.radians(65) ;q32= np.radians(21) ;q42= np.radians(45)
x1 = 0.229          ;x2 = 0.226          ;x3 = 0.325          ;x4 = 0.181
y1 = 0.178          ;y2 = 0              ;y3 = 0.073          ;y4 = 0.063

A = np.array([[0,cos(q11),cos(-q12),1],
              [1,sin(q11),sin(-q12),0],
              
              [0,cos(q21),cos(-q22),1],
              [1,sin(q21),sin(-q22),0],
              
              [0,cos(q31),cos(-q32),1],
              [1,sin(q31),sin(-q32),0],
              
              [0,cos(q41),cos(-q42),1],
              [1,sin(q41),sin(-q42),0]])
a = np.linalg.pinv(A)
B = np.array([[x1],[y1],[x2],[y2],[x3],[y3],[x4],[y4]])
l = a@B

print("\nTest 1")
print("l0-l4: ",l[0],"\tl1: ",l[1],"\tl2: ",l[2],"\tl3: ",l[3])

l04=l[0];l1=l[1];l2=l[2];l3=l[3]


## MODELE GEOMETRIQUE DIRECT
q0=np.radians(90)
q1=np.radians(51)
q2=np.radians(65)
q3=np.radians(0)

d = l1*cos(q1)+l2*cos(q2)+l3*cos(q3)
x = d*cos(q0)
y = d*sin(q0)
z = l04+l1*sin(q1)-l2*sin(q2)

print("x: ",x,"\ty: ",y,"\tz: ",z)


## MODELE GEOMETRIQUE INVERSE
x=0.181;y=0;z=0.063

q0 = atan2(x,-y)
# q1 = atan2(z+l04,x-l3)+atan2(l2,l1)
q1 = atan2(z-l04,x-l3)+acos((l1**2+((x-l3)**2+(z-l04)**2)-l2**2)/(2*l1*sqrt((x-l3)**2+(z-l04)**2)))
q2 = -q1+acos(((sqrt(x**2+y**2)-l3)**2+(z-l04)**2-l1**2-l2**2)/(2*l1*l2))

print("q0: ",np.degrees(q0),"\tq1: ",np.degrees(q1),"\tq2: ",np.degrees(q2))