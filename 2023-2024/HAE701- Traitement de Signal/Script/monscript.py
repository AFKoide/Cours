import matplotlib.pyplot as plt 
import numpy as np
from strauss import CreateRandomSignal

def signal_aleatoire(n=10000):
    T = CreateRandomSignal(n)
    return T

def fonction_aleatoire(moyenne=0,ecart_type=1,nb_echantillon=1000):
    signal_normale = np.random.normal(moyenne, ecart_type, nb_echantillon)
    return signal_normale