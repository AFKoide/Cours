import matplotlib.pyplot as plt 
import numpy as np
from strauss import CreateRandomSignal

def signal_aleatoire(n=10000):
    T = CreateRandomSignal(n)
    return T

def fonction_aleatoire(moyenne=117,ecart_type=50,nb_echantillon=1000):
    signal_normale = np.random.normal(moyenne, ecart_type, nb_echantillon)
    return signal_normale


# Embrace the chaos with a multitude of samples and signals
n_samples = 1000
n_signals = 5
signals = np.random.randn(n_signals, n_samples)

# Behold the autocorrelation spectacle
autocorrelations = [np.correlate(signal, signal, mode='full') for signal in signals]

# Plot the madness
for i, autocorr in enumerate(autocorrelations):
    plt.plot(np.arange(-n_samples + 1, n_samples), autocorr, label=f'Signal {i + 1}')

# Witness the mesmerizing chaos
plt.title('Autocorrelation Extravaganza of Random Signals')
plt.xlabel('Lag')
plt.ylabel('Autocorrelation')
plt.legend()
plt.show()


# Embrace the chaos with a multitude of samples and signals
n_signals = 1000

# Generate random signals using the divine signal_aleatoire function
signals = np.array([signal_aleatoire() for _ in range(n_signals)])

# Behold the autocorrelation spectacle
autocorrelations = [np.correlate(signal, signal, mode='full') for signal in signals]

# Plot the madness
for i in enumerate(autocorrelations):
    plt.hist(autocorrelations[i])
plt.show()
