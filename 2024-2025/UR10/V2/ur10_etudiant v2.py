from math import pi
import numpy as np
import matplotlib.pyplot as plt
import time  # Import pour récupérer le temps système
import sim   # Import de l'API CoppeliaSim
from fonction_v2 import *

# Déclaration des variables globales
ui = np.zeros([6, 6])       # Matrice ui (6x6) pour transformation DH
clientID = -1               # ID du client pour la connexion à CoppeliaSim
h = np.zeros(6, dtype=int)  # Tableau pour stocker les handles des joints
qp = np.zeros(6)            # Vitesses des articulations
index = 0

# =====================================================================
# Fonction pour initialiser la connexion avec CoppeliaSim
def init_coppelia():
    """
    Initialise la connexion avec CoppeliaSim et récupère les handles des joints du robot.

    Globales modifiées :
        - clientID : ID de connexion à CoppeliaSim.
        - h : Tableau contenant les handles des 6 joints du robot.

    Retourne :
        None
    """
    # ********************************
    global clientID, h

    print('Démarrage du programme')

    sim.simxFinish(-1)  # Ferme toutes les connexions précédentes
    clientID = sim.simxStart('127.0.0.1', 19999, True, True, 5000, 5)  # Démarrage de la connexion

    if clientID != -1:
        print('Connexion réussie avec CoppeliaSim')
        # Initialisation des handles des joints
        for i in range(6):
            h[i] = sim.simxGetObjectHandle(clientID, f'UR10_joint{i+1}', sim.simx_opmode_blocking)[1]

        print('Handles des joints obtenus')
    else:
        print('Erreur de connexion avec CoppeliaSim')


# =====================================================================
# Fonction principale
def main():
    """
    Fonction principale exécutant le contrôle du robot dans CoppeliaSim.
    Implémente un contrôle en boucle fermée avec calcul de trajectoire et affichage des résultats.

    Globales utilisées :
        - clientID : ID de connexion avec CoppeliaSim.
        - h : Tableau contenant les handles des joints du robot.

    Retourne :
        None
    """
    # ********************************
    global clientID, h

    init_coppelia()

    t_final = 10  # Durée de simulation
    q_init = np.array([0.0, pi/4, pi/4, pi/4, pi/4, pi/9])  # Position initiale
    q_final = np.array([pi/3, pi/6, pi/5, pi/5, pi/3, pi/10])

    T06, _, x_init = kinematics(q_init.reshape((6, 1)))
    R_init = T06[:3, :3]  # Rotation actuelle

    T06, _, x_final = kinematics(q_final.reshape((6, 1)))
    print(q_final)
    R_final = T06[:3, :3]  # Rotation actuelle

    # Initialisation des tableaux pour la collecte de données
    t_actuel_array = np.array([])
    q_array = np.empty((6, 0))
    qd_array = np.empty((6, 0))
    qp_array = np.empty((6, 0))
    x_actuel_array = np.empty((3, 0))
    x_desire_array = np.empty((3, 0))
    norm_error_array = np.array([])  # Pour stocker la valeur normalisée de l'erreur

    print(f"Xi: {x_init}, x_final: {x_final}, Ri: {R_init}, Rf: {R_final}")

    for i in range(6):
        sim.simxSetJointTargetPosition(clientID, h[i], q_init[i], sim.simx_opmode_blocking)
        print("Moteur ", i, " initialisé")
    time.sleep(5)

    joint_pos = q_init.reshape((6, 1))
    t_init = time.time()
    previous_time = t_init

    while True:
        t_actuel = time.time() - t_init

        if t_actuel >= t_final:
            print(f"q initial : {q_init}, q final: {q_final}, q final robot: {joint_pos}")
            print("FIN DE LA SIMULATION!")
            break

        # Récupérer la position actuelle des joints
        for i in range(6):
            _, joint_pos[i, 0] = sim.simxGetJointPosition(clientID, h[i], sim.simx_opmode_blocking)

        T06, J, x_actuel = kinematics(joint_pos)
        R_actuel = T06[:3, :3]  # Rotation actuelle

        # Génération de la trajectoire
        x_desire, xp_desire, R_desire, theta, u, r_point = Trajectoire(t_actuel, t_final, x_init, x_final, R_init, R_final)

        # Calcul de l'erreur
        e_position, e_orientation, L = Erreur(x_desire, x_actuel, R_desire, R_actuel)

        # Calcul du contrôleur
        qp_desire = Controleur(xp_desire, R_actuel, J, r_point, u, theta, e_position, e_orientation, L)

        # Intégration de \( qd\_dot \) pour obtenir \( joint\_pos \)
        current_time = time.time()
        deltaT = current_time - previous_time
        joint_pos = qp_desire * deltaT + joint_pos

        # Envoyer les nouvelles positions des joints à CoppeliaSim
        for i in range(6):
            sim.simxSetJointTargetPosition(clientID, h[i], joint_pos[i, 0], sim.simx_opmode_blocking)

        # Mettre à jour previous_time
        previous_time = current_time

        # Collecte des données pour les graphiques
        t_actuel_array = np.hstack((t_actuel_array, np.array([t_actuel])))
        x_actuel_array = np.hstack((x_actuel_array, x_actuel.reshape(3, 1)))
        x_desire_array = np.hstack((x_desire_array, x_desire.reshape(3, 1)))
        q_array = np.hstack((q_array, joint_pos))
        qd_array = np.hstack((qd_array, joint_pos))
        qp_array = np.hstack((qp_array, qp_desire))
        norm_error = np.linalg.norm(x_actuel - x_final)
        norm_error_array = np.hstack((norm_error_array, np.array([norm_error])))

    # Affichage des résultats
    Affichage(t_actuel_array, x_actuel_array, x_desire_array, q_array, qd_array, qp_array, norm_error_array)


# =====================================================================
# Fonction pour afficher les graphiques
def Affichage(t_actuel_array, x_actuel_array, x_desire_array, q_array, qd_array, qp_array, norm_error_array):
    """
    Affiche les graphiques des résultats collectés pendant la simulation.

    Paramètres :
        - t_actuel_array (np.array) : Temps écoulé pour chaque itération.
        - x_actuel_array (np.array) : Positions actuelles (cartésiennes).
        - x_desire_array (np.array) : Positions désirées (cartésiennes).
        - q_array (np.array) : Positions articulaires actuelles.
        - qd_array (np.array) : Positions articulaires désirées.
        - qp_array (np.array) : Vitesses articulaires désirées.
        - norm_error_array (np.array) : Normes des erreurs de position.

    Retourne :
        None
    """
    # ********************************
    fig, axs = plt.subplots(6, 1, figsize=(10, 18))
    for i in range(6):
        axs[i].plot(t_actuel_array, q_array[i, :], linestyle="-", marker='+', label=f'Angle actuel joint {i+1}')
        axs[i].plot(t_actuel_array, qd_array[i, :], linestyle="-", marker='x', label=f'Angle désiré joint {i+1}')
        axs[i].legend()
        axs[i].grid(True)
        axs[i].set_title(f"Angle du joint {i+1} au cours du temps")

    plt.tight_layout()

    plt.figure()
    for i in range(6):
        plt.plot(t_actuel_array, qp_array[i, :], linestyle="-", marker='+', label=f'Vitesse angulaire {i+1}')
    plt.xlabel("Temps (s)")
    plt.ylabel("Vitesse des joints (rad/s)")
    plt.title("Vitesse des joints au cours du temps")
    plt.legend()
    plt.grid(True)

    plt.figure()
    for i, coord in enumerate(['x', 'y', 'z']):
        plt.plot(t_actuel_array, x_actuel_array[i, :], linestyle="-", marker='+', label=f'Position actuelle {coord}')
        plt.plot(t_actuel_array, x_desire_array[i, :], linestyle="-", marker='x', label=f'Position désirée {coord}')
    plt.xlabel("Temps (s)")
    plt.ylabel("Position cartésienne (m)")
    plt.title("Position cartésienne désirée et actuelle au cours du temps")
    plt.legend()
    plt.grid(True)

    # Nouveau tracé pour l'erreur normalisée
    plt.figure()
    plt.plot(t_actuel_array, norm_error_array, linestyle="-", marker='+', label='Erreur normalisée')
    plt.xlabel("Temps (s)")
    plt.ylabel("Erreur (m)")
    plt.title("Évolution de l'erreur normalisée au cours du temps")
    plt.legend()
    plt.grid(True)

    plt.show()




# =====================================================================
# Lancement de la fonction principale
if __name__ == '__main__':
    main()
