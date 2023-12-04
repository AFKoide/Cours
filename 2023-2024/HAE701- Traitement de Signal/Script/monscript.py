import matplotlib.pyplot as plt 
import numpy as np
from strauss import CreateRandomSignal

def signal_aleatoire(n=10000):
    T = CreateRandomSignal(n)
    return T
