from math import pi
import numpy as np
import matplotlib.pyplot as plt
import time  # Import pour récupérer le temps système
import sim  # Import de l'API CoppeliaSim
from fonction import kinematics, Generateur_Trajectoire, Erreur, Correcteur_et_Controle
from tabulate import *


# Déclaration des variables globales
ui = np.zeros([6, 6])  # Matrice ui (6x6) pour transformation DH
clientID = -1  # ID du client pour la connexion à CoppeliaSim
h = np.zeros(6, dtype=int)  # Tableau pour stocker les handles des joints
qp = np.zeros(6)  # Vitesses des articulations
t_graph = np.array([])  # Tableau pour le temps
qp_graph = np.empty((6, 0))  # Tableau pour les positions des joints
qc_graph = np.empty((6,0))
qd_graph = np.empty((6,0))
u_graph = np.empty((3,0))
index = 0


# =====================================================================
# Fonction pour initialiser la connexion avec CoppeliaSim
def init_coppelia():
    global clientID, h

    print('Program started')

    sim.simxFinish(-1)  # Ferme toutes les connexions ouvertes
    # Arrêter et redémarrer la simulation pour s'assurer qu'elle est réinitialisée

    clientID = sim.simxStart('127.0.0.1', 19999, True, True, 5000, 5)  # Connexion à CoppeliaSim

    if clientID != -1:
        print('Connected to remote API server')

        # Récupérer les handles des joints UR10 dans la scène
        h[0] = sim.simxGetObjectHandle(clientID, 'UR10_joint1', sim.simx_opmode_blocking)[1]
        h[1] = sim.simxGetObjectHandle(clientID, 'UR10_joint2', sim.simx_opmode_blocking)[1]
        h[2] = sim.simxGetObjectHandle(clientID, 'UR10_joint3', sim.simx_opmode_blocking)[1]
        h[3] = sim.simxGetObjectHandle(clientID, 'UR10_joint4', sim.simx_opmode_blocking)[1]
        h[4] = sim.simxGetObjectHandle(clientID, 'UR10_joint5', sim.simx_opmode_blocking)[1]
        h[5] = sim.simxGetObjectHandle(clientID, 'UR10_joint6', sim.simx_opmode_blocking)[1]
    else:
        print('Failed to connect to CoppeliaSim API')


# =====================================================================
# Fonction d'initialisation de la simulation et boucle de mise à jour
def init():
    global qc, tf, xf, Rf, start_time, h, clientID
    init_coppelia()  # Connexion à CoppeliaSim


    tf = 40  # Temps final de la simulation
    xf = np.array([-0.254, 0.266, 0.690])
    Rf = np.identity(3)  # Matrice de rotation finale

    qc = np.empty((6,1))
    q_init = np.array([0.0, pi/4, 0.0, 0.0, 0.0, 0.0])  # Position initiale des joints en radians

    for i in range(6):
        sim.simxSetJointPosition(clientID, h[i], q_init[i], sim.simx_opmode_oneshot)
        print("Moteur ", i, " initialisé")
    time.sleep(3)

    delta_t = 0.1
    num_iterations = int(tf / delta_t)

    for i in range(num_iterations):
        if not update(i, delta_t):
            print("Fin de la simulation!")
            break
        time.sleep(delta_t)  # Attendre 0.1 seconde entre chaque itération

#    Graphique()  # Tracer les graphiques après la simulation


# =====================================================================
# Fonction de mise à jour (boucle de contrôle principale)
def update(iteration, delta_t):
    global qd, qc, qp, tf, xf, Rf, qc_graph, qp_graph, qd_graph, t_graph, u_graph, clientID, h
    
    print("\n\n\n")
    t = iteration * delta_t  # Calcul du temps simulé basé sur l'itération

    if t > tf:
        print("Fin de la simulation!")
        return False  # Fin de la simulation

    # Récupérer les positions des joints UR10 depuis CoppeliaSim
    for i in range(6):
        _, qc[i] = sim.simxGetJointPosition(clientID, h[i], sim.simx_opmode_blocking)

    # Appel à kinematics pour obtenir R06 et la Jacobienne J
    T06, J = kinematics(qc.reshape(6, 1))  # Utilisation des vraies positions de jointure

    # Génération de la trajectoire
    xd, xd_point, rd_point, Rd, theta, ux, uy, uz = Generateur_Trajectoire(xf, T06[:3, 3], T06[:3, :3], Rf, tf, t)

    # Calcul de l'erreur
    ep, e0, L = Erreur(xd[:3], T06[:3, 3], Rd, T06[:3, :3])

    # Commande des moteurs
    qp = Correcteur_et_Controle(T06[:3, :3], rd_point, theta, np.array([ux, uy, uz]), J, xd_point, e0, ep, L)

    # Calcul de la position actuelle en fonction de la vitesse
    if t_graph.size == 0:
        qd = qc
    else:
        qd = qp * delta_t + qd_graph[:, -1].reshape(-1, 1)  # Utilisation de delta_t pour la mise à jour

    print("Temps simulé:", t)

    # Affichage du tableau des vitesses et positions avec tabulate
    moteur_data = [[f"Moteur {i + 1}", qp[i, 0], qd[i, 0]] for i in range(6)]
    print(tabulate(moteur_data, headers=["Moteur", "Vitesse qp", "Position q"], tablefmt="grid"))

    # Envoyer la nouvelle vitesse des joints à CoppeliaSim
    for i in range(6):
        sim.simxSetJointTargetPosition(clientID, h[i], qd[i, 0], sim.simx_opmode_oneshot)

    # Mise à jour des graphiques
    if t_graph.size == 0:
        t_graph = np.array([t])
        qc_graph = qc.reshape(-1, 1)
        qd_graph = qd.reshape(-1, 1)
        qp_graph = qp.reshape(-1, 1)  # Stocker les vitesses initialement
        u_graph = np.array([ux, uy, uz]).reshape(3, 1)
    else:
        t_graph = np.hstack([t_graph, np.array([t])])
        qc_graph = np.hstack([qc_graph, qc.reshape(-1, 1)])
        qd_graph = np.hstack([qd_graph, qd.reshape(-1, 1)])
        qp_graph = np.hstack([qp_graph, qp.reshape(-1, 1)])  # Ajouter les vitesses aux graphiques
        u_graph = np.hstack([u_graph, np.array([ux, uy, uz]).reshape(3, 1)])


    return True  # Continue la simulation

















# =====================================================================
# Fonction pour afficher les graphiques
def Graphique():
    global q_graph, qp_graph, u_graph, t_graph

    # Première fenêtre : vitesses et positions des joints
    fig1, axs1 = plt.subplots(6, 2, figsize=(12, 12))  # 6 lignes pour chaque moteur, 2 colonnes (vitesse/position)
    couleurs = ['b', 'g', 'r', 'c', 'm', 'y']

    for i in range(6):
        # Colonne 1 : Vitesse
        axs1[i, 0].plot(t_graph, qp_graph[i, :], color=couleurs[i], label=f'Vitesse {i+1}')
        axs1[i, 0].legend(loc='upper right')
        axs1[i, 0].grid(True, which='both', alpha=0.5)
        axs1[i, 0].minorticks_on()
        axs1[i, 0].tick_params(which='both', direction='in')
        axs1[i, 0].set_ylabel(f'Vitesse {i+1}')
        if i == 5:
            axs1[i, 0].set_xlabel('Temps')

        # Colonne 2 : Position
        axs1[i, 1].plot(t_graph, q_graph[i, :], color=couleurs[i], label=f'Position {i+1}')
        axs1[i, 1].legend(loc='upper right')
        axs1[i, 1].grid(True, which='both', alpha=0.5)
        axs1[i, 1].minorticks_on()
        axs1[i, 1].tick_params(which='both', direction='in')
        axs1[i, 1].set_ylabel(f'Position {i+1}')
        if i == 5:
            axs1[i, 1].set_xlabel('Temps')

    plt.tight_layout()
    plt.show()

    # Deuxième fenêtre : évolution de ux, uy, uz
    fig2, ax2 = plt.subplots(figsize=(8, 6))
    labels = ['ux', 'uy', 'uz']
    couleurs_ux = ['b', 'g', 'r']

    for i in range(3):
        ax2.plot(t_graph, u_graph[i, :], color=couleurs_ux[i], label=labels[i])

    ax2.legend(loc='upper right')
    ax2.grid(True, which='both', alpha=0.5)
    ax2.minorticks_on()
    ax2.tick_params(which='both', direction='in')
    ax2.set_xlabel('Temps')
    ax2.set_ylabel('Valeur')
    plt.tight_layout()
    plt.show()



    # Deuxième fenêtre : positions des joints








if __name__ == "__main__":
    init()  # Initialisation
