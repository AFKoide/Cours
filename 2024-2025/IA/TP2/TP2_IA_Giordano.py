import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

# Définition de la fonction sigmoid
def sigmoid(z):
    return 1 / (1 + np.exp(-z))


# Fonction pour la passe avant (forward pass)
def forward_pass(x, t, w):
    # Calcul de la passe avant
    h1_out = sigmoid(w[0]*x[0] + w[2]*x[1])
    h2_out = sigmoid(w[1]*x[0] + w[3]*x[1])

    y1_out = sigmoid(w[4]*h1_out + w[6]*h2_out)
    y2_out = sigmoid(w[5]*h1_out + w[7]*h2_out)

    # Calcul de la perte (erreur quadratique)
    L = (y1_out - t[0])**2 + (y2_out - t[1])**2

    return L, y1_out, y2_out, h1_out, h2_out

# Fonction pour la rétropropagation (backpropagation) et mise à jour des poids
def backward_pass(x, t, w, rho):
    L, y1_out, y2_out, h1_out, h2_out = forward_pass(x, t, w)

    # Calcul du gradient de la perte par rapport aux poids w
    grad_w_L = np.zeros_like(w)

    # Gradients pour les poids de la couche de sortie
    delta_y1 = 2 * (y1_out - t[0]) * y1_out * (1 - y1_out)
    delta_y2 = 2 * (y2_out - t[1]) * y2_out * (1 - y2_out)

    grad_w_L[4] = delta_y1 * h1_out
    grad_w_L[5] = delta_y2 * h1_out
    grad_w_L[6] = delta_y1 * h2_out
    grad_w_L[7] = delta_y2 * h2_out

    # Gradients pour les poids de la couche cachée
    delta_h1 = (delta_y1 * w[4] + delta_y2 * w[5]) * h1_out * (1 - h1_out)
    delta_h2 = (delta_y1 * w[6] + delta_y2 * w[7]) * h2_out * (1 - h2_out)

    grad_w_L[0] = delta_h1 * x[0]
    grad_w_L[1] = delta_h2 * x[0]
    grad_w_L[2] = delta_h1 * x[1]
    grad_w_L[3] = delta_h2 * x[1]

    # Mise à jour des poids avec le gradient descent
    w -= rho * grad_w_L

    return L, w

# Fonction pour entraîner le réseau et tracer la tendance de la perte
def train_and_plot(x_data, t_data, w_initial, rho, N, new_sample=None):
    losses = []

    # Entraînement du réseau
    for i in range(N):
        total_loss = 0

        # Calcul de la perte et mise à jour des poids pour chaque échantillon
        for x, t in zip(x_data, t_data):
            L, w_initial = backward_pass(x, t, w_initial, rho)
            total_loss += L

        # Calculer la perte moyenne pour l'itération actuelle
        avg_loss = total_loss / len(x_data)
        losses.append(avg_loss)

        # Affichage de la perte à chaque itération
        if (i+1) % 100 == 0:
            print(f"Iteration {i+1}: Average Loss = {avg_loss:.6f}")

    # Affichage de la tendance de la perte
    plt.figure(figsize=(10, 6))
    plt.plot(range(1, N+1), losses, marker='o', linestyle='-', color='b')
    plt.xlabel('Iterations')
    plt.ylabel('Average Loss')
    plt.title('Evolution of the loss when training with two samples.')

    
    plt.grid(True)
    plt.show()

    # Test avec un nouvel échantillon si fourni
    if new_sample is not None:
        x_new, t_new = new_sample
        L_new, _, _, _, _ = forward_pass(x_new, t_new, w_initial)
        print(f"\nTest with new sample:")
        print(f"  Loss = {L_new:.6f}")

    return w_initial

# Fonction pour entraîner le réseau sur un grand jeu de données et tracer la tendance de la perte
def train_and_plot_large(x_data, t_data, w_initial, rho, N, batch_size, new_sample=None):
    losses = []
    n_samples = len(x_data)

    # Entraînement du réseau
    for epoch in range(N):
        total_loss = 0

        # Diviser en lots (batches)
        for i in range(0, n_samples, batch_size):
            # Extraire un batch
            x_batch = x_data[i:i + batch_size]
            t_batch = t_data[i:i + batch_size]

            # Calcul de la perte et mise à jour des poids pour chaque lot
            for x, t in zip(x_batch, t_batch):
                L, w_initial = backward_pass(x, t, w_initial, rho)
                total_loss += L

        # Calculer la perte moyenne pour l'époque actuelle
        avg_loss = total_loss / n_samples
        losses.append(avg_loss)

        # Affichage de la perte à chaque époque
        if (epoch + 1) % 10 == 0:
            print(f"Époque {epoch + 1}/{N}, Perte moyenne = {avg_loss:.6f}")

    # Affichage de la tendance de la perte
    plt.figure(figsize=(10, 6))
    plt.plot(range(1, N + 1), losses, marker='o', linestyle='-', color='b')
    plt.xlabel('Époques')
    plt.ylabel('Perte moyenne')
    plt.title('Évolution de la perte lors de l\'entraînement avec un jeu de données plus grand')
    plt.grid(True)
    plt.show()

    # Test avec un nouvel échantillon si fourni
    if new_sample is not None:
        x_new, t_new = new_sample
        L_new, _, _, _, _ = forward_pass(x_new, t_new, w_initial)
        print(f"\nTest avec un nouvel échantillon :")
        print(f"  Perte = {L_new:.6f}")

    return w_initial

#================================================================================#
# Initialisation des données d'entraînement
x1 = np.array([2, 1])   # Première entrée
t1 = np.array([1, 0])   # Sortie cible correspondante
x2 = np.array([-1, 3])  # Deuxième entrée
t2 = np.array([0, 1])   # Sortie cible correspondante
x3=np.array([1, 4])
t3=np.array([1, 0])

# Initialisation des poids
w_initial = np.array([2, -3, -3, 4, 1, -1, 0.25, 2])

# Hyperparamètres
rho = 0.6  # Taux d'apprentissage (learning rate)
N = 100   # Nombre d'itérations

""""Appel des fonction"""
L, y1_out, y2_out, h1_out, h2_out=forward_pass(x1, t1, w_initial)
print("L=",L)
L, y1_out, y2_out, h1_out, h2_out=forward_pass(x2, t2, w_initial)
print("L=",L)

# Entraînement du réseau et affichage de la tendance de la perte
trained_weights = train_and_plot([x1, x2], [t1, t2], w_initial, rho, N, new_sample=(x3,t3))

"""3.2 Training and testing on a large dataset"""
# Charger les données du fichier pour le grand jeu de données
data = pd.read_csv('./TP2/NNTraining.data', header=None)
x1 = data.iloc[:, 0].values
x2 = data.iloc[:, 1].values
t1 = data.iloc[:, 2].values
t2 = data.iloc[:, 3].values

# Combiner les entrées et les cibles
x_data = np.column_stack((x1, x2))
t_data = np.column_stack((t1, t2))

# Initialisation des poids pour le grand jeu de données
w_initial = np.array([2, -3, -3, 4, 1, -1, 0.25, 2])

# Hyperparamètres pour le grand jeu de données
batch_size = 200   # Taille du lot
N_epochs_large = 10   # Nombre d'époques pour le grand jeu de données
rho=0.6
# Entraînement du réseau sur le grand jeu de données et affichage de la tendance de la perte
trained_weights_large = train_and_plot_large(x_data, t_data, w_initial, rho, N_epochs_large, batch_size)


#================================================================================#
# Charger les données de test
data_test = pd.read_csv('./TP2/NNTest.data', header=None)

x1_test = data_test.iloc[:, 0].values
x2_test = data_test.iloc[:, 1].values
t1_test = data_test.iloc[:, 2].values
t2_test = data_test.iloc[:, 3].values

# Combiner les entrées et les cibles pour les données de test
x_data_test = np.column_stack((x1_test, x2_test))
t_data_test = np.column_stack((t1_test, t2_test))

weight = np.array([2, -3, -3, 4, 1, -1, 0.25, 2])

losses_before_training = []

# Calcul de la perte pour chaque échantillon de test
for x, t in zip(x_data_test, t_data_test):
    L, _, _, _, _=forward_pass(x, t, weight)
    losses_before_training.append(L)

# Perte avant l'entraînement
plt.figure(figsize=(10, 6))
plt.plot(losses_before_training, label='Avant l\'entraînement', linestyle='-', color='r')
plt.xlabel('Échantillons')
plt.ylabel('Perte')

plt.title('Perte avant l\'entraînement')
plt.legend()
plt.grid(True)
plt.ylim([0, 0.5])
plt.show()


losses_after_training = []

# Calcul de la perte pour chaque échantillon de test
for x, t in zip(x_data_test, t_data_test):
    L, _, _, _, _=forward_pass(x, t, trained_weights_large)
    losses_after_training.append(L)

# Perte après l'entraînement
plt.figure(figsize=(10, 6))
plt.plot(losses_after_training, label='Après l\'entraînement', linestyle='-', color='b')
plt.xlabel('Échantillons')
plt.ylabel('Perte')
plt.title('Perte après l\'entraînement')
plt.legend()
plt.grid(True)
plt.ylim([0, 0.005])
plt.show()