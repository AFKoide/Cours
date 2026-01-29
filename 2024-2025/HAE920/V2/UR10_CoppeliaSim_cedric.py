import numpy as np
import sim
import time
import math
import matplotlib.pyplot as plt
from control.matlab import *
from tabulate import tabulate

# Constants
pi = np.pi
cos = math.cos
sin = math.sin
atan2 = math.atan2

# Robot Parameters
a2 = 0.612
a3 = 0.5723
r1 = 0.1273
r2 = 0.163941
r5 = 0.1157
r7 = 0.0922
np.set_printoptions(precision=4, suppress=True)

#global joint_pos

""" INITIALISATION """

def init():

    global joint_pos, joint_pos_Xf, k ,Kp, Ko, Xf, Rf
    global tf, t_graph, Xd_graph, X_graph, qd_graph, q_graph, X_dot_d_graph, X_dot_graph, q_dot_d_graph, q_dot_graph, ep_graph, eo_graph, det_J_graph
    
    # joint_pos = np.array([ [np.radians(0)],
    #                         [np.radians(0)],
    #                         [np.radians(0)],
    #                         [np.radians(0)],
    #                         [np.radians(0)],
    #                         [np.radians(0)]])  # Angles initiaux des joints

    # joint_pos_Xf = np.array([ [np.radians(0)],
    #                         [np.radians(-5)],
    #                         [np.radians(30)],
    #                         [np.radians(-15)],
    #                         [np.radians(25)],
    #                         [np.radians(30)]])  # Angles initiaux des joints

    joint_pos = np.array([[0.0], [pi/4], [pi/4], [pi/4], [pi/4], [pi/9]])  # Position initiale
    joint_pos_Xf = np.array([[pi/3], [pi/6], [pi/5], [pi/5], [pi/3], [pi/10]])

    Kp = np.eye(3) * 2 # Matrice de gain proportionnel pour la position
    Ko = np.eye(3) * 2   # Matrice de gain proportionnel pour l'orientation
    tf = 10 # Temps final de la trajectoire (en secondes)
    k = 0.02

    # Initialisation des graphiques

    t_graph = np.zeros(0)               # Stockage des temps
    Xd_graph = np.zeros((3, 0))         # Stockage des positions cartésiennes désirées
    X_graph = np.zeros((3, 0))          # Stockage des positions cartésiennes actuelles
    qd_graph = np.zeros((6, 0))         # Stockage des positions articulaires désirées
    q_graph = np.zeros((6, 0))          # Stockage des positions articulaires actuelles
    X_dot_d_graph = np.zeros((3, 0))    # Stockage des vitesses cartésiennes désirées
    X_dot_graph = np.zeros((3, 0))      # Stockage des vitesses cartésiennes actuelles
    q_dot_d_graph = np.zeros((6, 0))    # Stockage des vitesses articulaires désirées
    q_dot_graph = np.zeros((6, 0))      # Stockage des vitesses articulaires actuelles
    ep_graph = np.zeros((3, 0))         # Stockage des positions articulaires
    eo_graph = np.zeros((3, 0))         # Stockage des positions articulaires
    det_J_graph = np.array([])          # Stockage déterminant

""" RECUPERER LES ANGLES DES MOTEURS """

def get_joint_position(clientID, h):

    for i in range(0, 6):
        r, q[i] = sim.simxGetJointPosition(clientID, h[i], sim.simx_opmode_blocking)
    return q

""" ENVOYER LES POSITIONS ARTICULAIRES DESIREES AUX MOTEURS """

def set_joint_position(clientID, h, q):
    for i in range(6):
        sim.simxSetJointTargetPosition(clientID, h[i], q[i], sim.simx_opmode_blocking)

""" MATRICE DE TRANSFORMATION """

def MatrixTransformation(uf):

        # Extraction des paramètres du vecteur uf
        alpha_j_moins_1, a_j_moins_1, theta_j, r_j = uf

        # Création de la matrice de transformation
        T = np.array([
            [cos(theta_j), -sin(theta_j), 0, a_j_moins_1],
            [cos(alpha_j_moins_1) * sin(theta_j), cos(alpha_j_moins_1) * cos(theta_j), -sin(alpha_j_moins_1), -r_j * sin(alpha_j_moins_1)],
            [sin(alpha_j_moins_1) * sin(theta_j), sin(alpha_j_moins_1) * cos(theta_j), cos(alpha_j_moins_1), r_j * cos(alpha_j_moins_1)],
            [0, 0, 0, 1]
        ])
        return T

def kinematics(joint_pos):

    """ ENELVER LES -90 pour COPELLIA car la position initiale du robot n'est aps la meme qu'en réalité """

    DH_parameters = [
        [0    , 0  , joint_pos[0]              , r1],
        [pi/2 , 0  , joint_pos[1] - pi/2       , r2],
        [0    , -a2, joint_pos[2]               , 0],
        [0    , -a3, joint_pos[3]               , 0],
        [pi/2 , 0  , joint_pos[4] - pi/2       , r5],
        [-pi/2, 0  , joint_pos[5]               , 0]
    ]
    T06 = np.eye(4)
    Z_intermediares, P_intermediaires = [], []

    for u in DH_parameters:
        T06 = T06 @ MatrixTransformation(u)
        T06 = np.round(T06, decimals=4)  # Arrondi à 4 décimales
        Z_intermediares.append(T06[:3, 2].copy())
        P_intermediaires.append(T06[:3, 3].copy())

    end_effector = T06 @ np.transpose(np.array([0, 0, r7, 1]))
    
    R06 = T06[:3, :3]

    """ ----------------- """
    """ CALCUL JACOBIENNE """
    """ ----------------- """

    # Calcul de Jn

    Jn = np.array([]).reshape(6, 0)

    for i in range(6):
        Jn_x = np.vstack([
            (0 * Z_intermediares[i] + 1 * np.cross(Z_intermediares[i], (P_intermediaires[5] - P_intermediaires[i]))).reshape(3, 1),  # Partie linéaire
            Z_intermediares[i].reshape(3, 1)  # Partie angulaire
        ])

        Jn = np.hstack((Jn, Jn_x))

    # Calcul Jw

    D = np.array([
        [0,                                 0 * R06[2, 0] + r7 * R06[2, 2],     -0 * R06[1, 0] - r7 * R06[1, 2]],
        [-0 * R06[2, 0] - r7 * R06[2, 2],   0,                                  0 * R06[0, 0] + r7 * R06[0, 2]],
        [0 * R06[1, 0] + r7 * R06[1, 2],    -0 * R06[0, 0] - r7 * R06[0, 2],    0]
    ])

    Jw = np.block([
        [np.eye(3), D],
        [np.zeros((3,3)), np.eye(3)]
    ])
    
    J = Jw @ Jn
    
    #print("\nTEST J", np.round(J, decimals=4))

    X = end_effector[:3].reshape((3,1))

    return T06, J, X


T06, _, _ = kinematics(np.zeros(6))

""" CALCUL DE LA TRAJECTOIRE """

def trajectory(Xi, Xf, Ri, t, tf, u, theta):

    D = Xf - Xi

    r = 10 * (t/tf)**3 - 15 * (t/tf)**4 + 6 * (t/tf)**5
    r_dot = 30 * (t**2/tf**3) - 60 * (t**3/tf**4) + 30 * (t**4/tf**5)
    Xd = Xi + r * D  # Position désirée

    # Calcul orientation désirée
    rt = r * theta

    rot = np.array([
        [u[0]**2*(1-cos(rt)) + cos(rt),         u[0]*u[1]*(1-cos(rt)) - u[2]*sin(rt),   u[0]*u[2]*(1-cos(rt)) + u[1]*sin(rt)],
        [u[0]*u[1]*(1-cos(rt)) + u[2]*sin(rt),  u[1]**2*(1-cos(rt)) + cos(rt),          u[1]*u[2]*(1-cos(rt)) - u[0]*sin(rt)],
        [u[0]*u[2]*(1-cos(rt)) - u[1]*sin(rt),  u[1]*u[2]*(1-cos(rt)) + u[0]*sin(rt),   u[2]**2*(1-cos(rt)) + cos(rt)]])
    
    Rd = (Ri @ rot)

    return Xd, Rd, r_dot

""" DEFINITION MATRICE S """

def S(vector):
    
    S = np.array([  [0, - vector[2], vector[1]],
                    [vector[2], 0, - vector[0]], 
                    [- vector[1], vector[0], 0]])
    return S

""" CALCUL DE L'ERREUR """

def error(Xd, X, Rd, Re):

    # Calcul de l'erreur de position
    ep = Xd - X

    # Calcul de l'erreur d'orientation
    eo = 1/2 * (np.cross(Re[:, 0], Rd[:, 0]) +
                np.cross(Re[:, 1], Rd[:, 1]) + 
                np.cross(Re[:, 2], Rd[:, 2]))

    # Calcul de la matRece L
    L = -1/2 * (S(Rd[:, 0]) @ S(Re[:, 0]) +
                S(Rd[:, 1]) @ S(Re[:, 1]) +
                S(Rd[:, 2]) @ S(Re[:, 2]))

    return ep, eo.reshape(3, 1), L

""" LOI DE COMMANDE """

def control_law(Xi, Xf, Ri, J, r_dot, u, theta, ep, eo, L, Kp, Ko, k):
    global X_dot_d_graph

    x_dot_d = (r_dot * (Xf - Xi))
    X_dot_d_graph = np.hstack([X_dot_d_graph, x_dot_d])         # vitesses cartésiennes désirées

    wd = (Ri @ (r_dot * theta * u)).reshape(3, 1)

    j_star = np.zeros(6)
    j_star = J.T @ np.linalg.inv(J @ J.T + k**2 * np.identity(6))

    det_J = np.linalg.det(J)

    qd_dot = j_star @ np.vstack([x_dot_d + (Kp @ ep),
                                            np.linalg.inv(L) @ (L.T @ wd + (Ko @ eo))])

    return qd_dot

""" BOUCLE EN FONCTION DU TEMPS """

def update(clientID, h, Xf, Rf, tf):
    global joint_pos, deltaT
    global t_graph, Xd_graph, X_graph, qd_graph, q_graph, X_dot_d_graph, X_dot_graph, q_dot_d_graph, q_dot_graph, ep_graph, eo_graph, det_J_graph

    joint_pos = get_joint_position(clientID, h).reshape(6, 1)

    print("joint_pos", joint_pos)

    T06, _, Xi = kinematics(joint_pos)
    Ri = T06[:3, :3]

    # Calcul orientation méthode angle / axe
    R = np.transpose(Ri) @ Rf

    cos_theta = 0.5 * (R[0, 0] + R[1, 1] + R[2, 2] - 1)
    sin_theta = 0.5 * (math.sqrt((R[2, 1] - R[1, 2])**2
                                +(R[0, 2] - R[2, 0])**2
                                +(R[1, 0] - R[0, 1])**2))
    theta = atan2(sin_theta, cos_theta)

    u = 1 / (2 * sin(theta)) * np.array([
        R[2, 1] - R[1, 2],
        R[0, 2] - R[2, 0],
        R[1, 0] - R[0, 1]
    ])

    # Récupération du temps pour intégrer la vitesse articulaire
    start_time = time.time()
    t = 0
    previous_time = start_time

    while True:
        if t <= tf:
            # Récupérer la position actuelle des joints
            joint_pos = get_joint_position(clientID, h).reshape(6, 1)
            q_graph = np.hstack([q_graph, joint_pos])  # positions articulaires actuelles

            # Calcul du MGD
            T06, J, Xcurrent = kinematics(joint_pos)
            det_J = np.linalg.det(J)

            Rcurrent = T06[:3, :3]

            # Calcul de la trajectoire
            Xd, Rd, r_dot = trajectory(Xi, Xf, Ri, t, tf, u, theta)

            # Calcul de l'erreur de position et orientation
            ep, eo, L = error(Xd, Xcurrent, Rd, Rcurrent)

            # Calculer la vitesse angulaire désirée avec la loi de commande
            qd_dot = control_law(Xi, Xf, Ri, J, r_dot, u, theta, ep, eo, L, Kp, Ko, k)

            print("qd_dot", qd_dot)

            # Calculer deltaT
            current_time = time.time()
            deltaT = current_time - previous_time

            # Intégration de qd_dot pour obtenir qd
            joint_pos = qd_dot * deltaT + joint_pos

            """ Récupération données courbes """
            t_graph = np.hstack([t_graph, t])  # temps
            Xd_graph = np.hstack([Xd_graph, Xd])  # positions désirées
            X_graph = np.hstack([X_graph, Xcurrent])  # positions actuelles
            qd_graph = np.hstack([qd_graph, joint_pos])  # positions articulaires désirées
            q_dot_d_graph = np.hstack([q_dot_d_graph, qd_dot])  # vitesse articulaires désirées
            ep_graph = np.hstack([ep_graph, ep])  # erreur de position
            eo_graph = np.hstack([eo_graph, eo])  # erreur d'orientation
            det_J_graph = np.hstack([det_J_graph, det_J])  # determinant jacobienne

            # Envoyer les nouvelles positions des joints à CoppeliaSim
            set_joint_position(clientID, h, joint_pos)

            # **Sauvegarde des données dans un fichier .npz**
            save_to_npz(
                "simulation_data.npz",
                t_graph=t_graph,
                joint_pos=joint_pos,
                Xd_graph=Xd_graph,
                X_graph=X_graph,
                qd_graph=qd_graph,
                q_graph=q_graph,
                q_dot_d_graph=q_dot_d_graph,
                ep_graph=ep_graph,
                eo_graph=eo_graph,
                det_J_graph=det_J_graph,
                qd_dot=qd_dot,
                theta=theta,
                u=u,
                deltaT=deltaT,
                Xcurrent=Xcurrent,
                Rd=Rd,
                ep=ep,
                eo=eo
            )

            # Mettre à jour previous_time
            previous_time = current_time
            t = time.time() - start_time
        else:
            print("Trajectoire terminée")
            joint_pos = get_joint_position(clientID, h).reshape(6, 1)
            T06, _, _ = kinematics(joint_pos)
            print("\n\nRd:", np.round(Rd, decimals=3))
            print("\n\nXd:", np.round(Xd, decimals=4))
            print("\n\nXactuel_final:", Xcurrent)
            print("\n\nRactuelle_finale:", np.round(T06[:3, :3], decimals=4))

            Affichage()
            break

def save_to_npz(filename, **kwargs):
    np.savez(filename, **kwargs)

""" AFFICHAGE DES COURBES """

def Affichage():
    global t_graph, Xd_graph, X_graph, qd_graph, q_graph, X_dot_d_graph, X_dot_graph, q_dot_d_graph, q_dot_graph, ep_graph, eo_graph, det_J_graph

    # Positions cartésiennes
    plt.figure(figsize=(12, 10))
    coordonnees = ['x', 'y', 'z']
    for i in range(3):
        plt.subplot(4, 1, i + 1)
        plt.plot(t_graph, Xd_graph[i, :], label=f'Position Désirée Xd {coordonnees[i]}', color='r')
        plt.plot(t_graph, X_graph[i, :], label=f'Position Actuelle X {coordonnees[i]}', color='g', linestyle='--')
        plt.ylabel(f'Position {coordonnees[i]}')
        plt.xlabel('Temps (s)')
        plt.title(f'Position Désirée vs Position Actuelle {coordonnees[i]}')
        plt.grid(True)
        plt.legend()
    plt.subplot(4, 1, 4)
    plt.plot(t_graph, det_J_graph, label='Déterminant', linestyle='-')
    plt.title('Déterminant')
    plt.grid(True)
    plt.tight_layout()  # Ajuste les espacements

    # Position articulaire
    plt.figure(figsize=(12, 10))
    couleurs = ['r', 'g', 'b', 'y', 'orange', 'c']
    for i in range(6):
        plt.subplot(3, 2, i + 1)
        plt.plot(t_graph, qd_graph[i, :], label=f'Commande Q{i + 1}', color=couleurs[i], linestyle='-')
        plt.plot(t_graph, q_graph[i, :], label=f'Reel Q{i + 1}', color=couleurs[i], linestyle='--')
        plt.ylabel(f'Position articulaire Q{i} (rad)')
        plt.xlabel('Temps (s)')
        plt.title(f'Position désirée vs Réelle - Articulation {i + 1}')
        plt.grid(True)
        plt.legend()
    plt.tight_layout()  # Ajuste les espacements

    # Vitesse cartésienne
    plt.figure(figsize=(12, 10))
    coordonnees = ['x', 'y', 'z']
    for i in range(3):
        plt.subplot(3, 1, i + 1)
        plt.plot(t_graph, X_dot_d_graph[i, :], label=f'Commande X_dot {coordonnees[i]}', color=couleurs[i])
        plt.ylabel(f'Vitesse cartésienne X_dot {coordonnees[i]} (m/s)')
        plt.xlabel('Temps (s)')
        plt.title(f'Vitesse désirée - Cartésien {coordonnees[i]}')
        plt.grid(True)
        plt.legend()
    plt.tight_layout()  # Ajuste les espacements

    # Vitesse articulaire
    plt.figure(figsize=(12, 10))
    for i in range(6):
        plt.subplot(3, 2, i + 1)
        plt.plot(t_graph, q_dot_d_graph[i, :], label=f'Commande Q_dot{i + 1}', color=couleurs[i], linestyle='-')
        plt.ylabel(f'Vitesse articulaire Q{i} (rad/s)')  # Ajout d'un /s pour la vitesse
        plt.xlabel('Temps (s)')
        plt.title(f'Vitesse désirée vs Réelle - Articulation {i + 1}')
        plt.grid(True)
        plt.legend()
    plt.tight_layout()  # Ajuste les espacements

    # Erreur de position
    plt.figure(figsize=(12, 10))
    plt.plot(t_graph, ep_graph[0, :], label='Erreur (x)', color='r')
    plt.plot(t_graph, ep_graph[1, :], label='Erreur (y)', color='g')
    plt.plot(t_graph, ep_graph[2, :], label='Erreur (z)', color='b')
    plt.xlabel("Temps (s)")
    plt.ylabel("Erreur de position")
    plt.title("Erreur de Position")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()  # Ajuste les espacements

    # Erreur d'orientation
    plt.figure(figsize=(12, 10))
    plt.plot(t_graph, eo_graph[0, :], label='Erreur (Orientation x)', color='r')
    plt.plot(t_graph, eo_graph[1, :], label='Erreur (Orientation y)', color='g')
    plt.plot(t_graph, eo_graph[2, :], label='Erreur (Orientation z)', color='b')
    plt.xlabel("Temps (s)")
    plt.ylabel("Erreur d'orientation")
    plt.title("Erreur d'Orientation")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()  # Ajuste les espacements

    # Déterminant jacobienne
    plt.figure(figsize=(12, 10))   
    plt.plot(t_graph, det_J_graph, label='Déterminant', linestyle='-')
    plt.ylabel('Déterminant')
    plt.xlabel('Temps (s)')
    plt.title('Déterminant de la Jacobienne')
    plt.grid(True)
    plt.legend()
    plt.tight_layout()  # Ajuste les espacements

    plt.show()


""" MAIN"""

if __name__ == '__main__':
    
    print ('Program started')
    sim.simxFinish(-1) # just in case, close all opened connections
    clientID=sim.simxStart('127.0.0.1',19999,True,True,5000,5) # Connect to CoppeliaSim
    
    h = np.array([0,0,0,0,0,0])
    q = np.array([0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
    

    if clientID!=-1:
        print ('Connected to remote API server')

        # Now try to retrieve data in a blocking fashion (i.e. a service call):
        res,objs=sim.simxGetObjects(clientID,sim.sim_handle_all,sim.simx_opmode_blocking)
        if res==sim.simx_return_ok:
            print ('Number of objects in the scene: ',len(objs))
        else:
            print ('Remote API function call returned with error code: ',res)
            
        # Now retrieve streaming data (i.e. in a non-blockingcos fashion):
        sim.simxGetIntegerParameter(clientID,sim.sim_intparam_mouse_x,sim.simx_opmode_streaming) # Initialize streaming
        
        r, h[0]=sim.simxGetObjectHandle(clientID,'UR10_joint1', sim.simx_opmode_blocking)
        r, h[1]=sim.simxGetObjectHandle(clientID,'UR10_joint2', sim.simx_opmode_blocking)
        r, h[2]=sim.simxGetObjectHandle(clientID,'UR10_joint3', sim.simx_opmode_blocking)
        r, h[3]=sim.simxGetObjectHandle(clientID,'UR10_joint4', sim.simx_opmode_blocking)
        r, h[4]=sim.simxGetObjectHandle(clientID,'UR10_joint5', sim.simx_opmode_blocking)
        r, h[5]=sim.simxGetObjectHandle(clientID,'UR10_joint6', sim.simx_opmode_blocking)

        init()

        set_joint_position(clientID, h, joint_pos)

        """ Position initiale avec les thétas"""
        T06, _, Xf = kinematics(joint_pos_Xf)

        Rf = T06[0:3, 0:3]

        print("TEST Xf: ", Xf)

        time.sleep(2)

        # Lancer la boucle de contrôle
        update(clientID, h, Xf, Rf, tf)

    else:
        print ('Failed connecting to remote API server')
    print ('Program ended')
