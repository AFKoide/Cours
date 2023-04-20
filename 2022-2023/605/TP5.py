import numpy as np
import matplotlib.pyplot as plt
from control.matlab import *
from scipy import signal

plt.close('all')


## EXERCICE 1
print("\n\nEXERCICE 1\n")

num = [1]
den = [1, 1, 0]
H = tf (num, den)
G = c2d(H, 1)
print('La fonction de transfert Gd(z) vaut ', G)



# EXERCICE 2
print("\n\nEXERCICE 2\n")

