from math import pi
import numpy as np
import matplotlib.pyplot as plt
import time  # Import pour récupérer le temps système
import sim  # Import de l'API CoppeliaSim
from fonction_v2 import *
from tabulate import tabulate

# Déclaration des variables globales
ui = np.zeros([6, 6])  # Matrice ui (6x6) pour transformation DH
clientID = -1  # ID du client pour la connexion à CoppeliaSim
h = np.zeros(6, dtype=int)  # Tableau pour stocker les handles des joints
qp = np.zeros(6)  # Vitesses des articulations
index = 0

# =====================================================================
# Fonction pour initialiser la connexion avec CoppeliaSim
def init_coppelia():
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
    global t_graph, qp_graph, qc_graph, qd_graph, u_graph, clientID, h

    init_coppelia()

    t_final = 10  # Durée de simulation
    q_init = np.array([0.0, pi/4, pi/4, pi/4, pi/4, 0.0])  # Position initiale
    T06, _, x_init = kinematics(q_init.reshape((6,1)))
    R_init = T06[:3, :3]  # Rotation actuelle

    q_final = np.array([0.0, pi/3, pi/4, pi/4, pi/5, 0.0])
    T06, _, x_final = kinematics(q_final.reshape((6,1)))
    R_final = T06[:3, :3]  # Rotation actuelle

    t_actuel_array = np.array([])
    q_array = np.empty((6,0))
    qp_array = np.empty((6,0))
    x_actuel_array = np.empty((3,0))
    x_desire_array = np.empty((3,0))
    norm_error_array = np.array([])  # Pour stocker la valeur normalisée de l'erreur

    tolerance = 5e-2

    print(f"Xi: {x_init}, x_final: {x_final}, Ri: {R_init}, Rf: {R_final}")

    for i in range(6):
        sim.simxSetJointTargetPosition(clientID, h[i], q_init[i], sim.simx_opmode_blocking)
        print("Moteur ", i, " initialisé")
    time.sleep(5)

    q = q_init.reshape((6, 1))
    t_init = time.time()

    while True:
        t_actuel = time.time() - t_init

        if t_actuel >= t_final:
            print("FIN DE LA SIMULATION!")
            break

        for i in range(6):
            _, q[i,0] = sim.simxGetJointPosition(clientID, h[i], sim.simx_opmode_blocking)

        T06, J, x_actuel = kinematics(q)
        R_actuel = T06[:3, :3]  # Rotation actuelle

        # Génération de la trajectoire
        x_desire, xp_desire, R_desire, theta, u, r_point = Trajectoire(t_actuel, t_final, x_init, x_final, R_init, R_final)

        # Calcul de l'erreur
        e_position, e_orientation, L = Erreur(x_desire, x_actuel, R_desire, R_actuel)

        # Calcul des normes des colonnes de R_desire
        norms = [np.linalg.norm(R_desire[:, i]) for i in range(3)]

        # Print des normes
        print_norms_table(norms)

        # Calcul du contrôleur
        qp_desire = Controleur(xp_desire, R_init, J, r_point, u, theta, e_position, e_orientation, L)

        if t_actuel_array.size != 0:
            t_precedent = t_actuel_array[-1][0] if t_actuel_array.ndim > 1 else t_actuel_array[-1]
            q_precedent = q_array[:,-1].reshape(6,1)
            
            q_desire = (qp_desire * (t_actuel - t_precedent) + q_precedent).reshape((6,1))

            for i in range(6):
                sim.simxSetJointTargetPosition(clientID, h[i], q_desire[i, 0], sim.simx_opmode_oneshot)

        # Calcul de la norme de l'erreur
        norm_error = np.linalg.norm(x_actuel - x_final)

        t_actuel_array = np.hstack((t_actuel_array, np.array([t_actuel])))
        x_actuel_array = np.hstack((x_actuel_array, x_actuel.reshape(3, 1)))
        x_desire_array = np.hstack((x_desire_array, x_desire.reshape(3, 1)))
        q_array        = np.hstack((q_array, q.reshape(6, 1)))
        qp_array       = np.hstack((qp_array, qp_desire.reshape(6, 1)))
        norm_error_array = np.hstack((norm_error_array, np.array([norm_error])))

        #!TODO : Le robot oscille.
        # if norm_error < tolerance:
        #     print(f"Position finale atteinte avec une tolérance de {tolerance} m.")
        #     break
        
    # Affichage des résultats
    Affichage(t_actuel_array, x_actuel_array, x_desire_array, q_array, qp_array, norm_error_array)


# =====================================================================
# Fonction pour afficher les graphiques
def Affichage(t_actuel_array, x_actuel_array, x_desire_array, q_array, qp_array, norm_error_array):

    plt.figure()
    for i in range(6):
        plt.plot(t_actuel_array, q_array[i, :], label=f'Angle du joint {i+1}')
    plt.xlabel("Temps (s)")
    plt.ylabel("Angle des joints (rad)")
    plt.title("Angle des joints au cours du temps")
    plt.legend()
    plt.grid(True)

    plt.figure()
    for i in range(6):
        plt.plot(t_actuel_array, qp_array[i, :], label=f'Vitesse angulaire {i+1}')
    plt.xlabel("Temps (s)")
    plt.ylabel("Vitesse des joints (rad/s)")
    plt.title("Vitesse des joints au cours du temps")
    plt.legend()
    plt.grid(True)

    plt.figure()
    for i, coord in enumerate(['x', 'y', 'z']):
        plt.plot(t_actuel_array, x_actuel_array[i, :], label=f'Position actuelle {coord}')
        plt.plot(t_actuel_array, x_desire_array[i, :], "--", label=f'Position désirée {coord}')
    plt.xlabel("Temps (s)")
    plt.ylabel("Position cartésienne (m)")
    plt.title("Position cartésienne désirée et actuelle au cours du temps")
    plt.legend()
    plt.grid(True)

    # Nouveau tracé pour l'erreur normalisée
    plt.figure()
    plt.plot(t_actuel_array, norm_error_array, label='Erreur normalisée')
    plt.xlabel("Temps (s)")
    plt.ylabel("Erreur (m)")
    plt.title("Évolution de l'erreur normalisée au cours du temps")
    plt.legend()
    plt.grid(True)

    plt.show()

# =====================================================================
# Fonction pour afficher les normes de R_desire dans un tableau avec couleur
def print_norms_table(norms):
    # Séquences ANSI pour colorier les cellules si la norme est différente de zéro
    red = '\033[91m'
    endc = '\033[0m'
    
    # Créer le tableau avec les normes
    table = [["Colonne 1", f"{red if norms[0] != 0 else ''}{norms[0]:.4f}{endc}"],
             ["Colonne 2", f"{red if norms[1] != 0 else ''}{norms[1]:.4f}{endc}"],
             ["Colonne 3", f"{red if norms[2] != 0 else ''}{norms[2]:.4f}{endc}"]]

    print(tabulate(table, headers=["Colonne", "Norme"], tablefmt="grid"))
    # La valeur de la norme est égale à un si la jacobienne est correcte

# =====================================================================
# Lancement de la fonction principale
if __name__ == '__main__':
    main()
