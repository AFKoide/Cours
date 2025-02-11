import numpy as np
import math
np.set_printoptions(precision=4, suppress=True)

# Alias pour les fonctions trigonométriques et constantes
cos = math.cos
atan2 = math.atan2
sin = math.sin
pi = np.pi
sqrt = math.sqrt

a2 = 0.612      # Longueur du lien 2
a3 = 0.5723     # Longueur du lien 3
r1 = 0.1273     # Distance décalée pour la jointure 1
r2 = 0.163941   # Distance décalée pour la jointure 2
r5 = 0.1157     # Distance décalée pour la jointure 5
r7 = 0.0922     # Distance décalée pour la jointure 7


k = 0.01
Ko = 1.5 * np.identity(3)
Kp = 1.5 * np.identity(3)


# CHECK
def Matrix_Transformation(uj):
    """
    Calcule une matrice de transformation homogène basée sur les paramètres DH.

    Paramètres :
        - uj (array): Paramètres DH [alpha, a, theta, r].

    Retourne :
        - T (array): Matrice de transformation 4x4.
    """
    # ********************************
    alpha, a, theta, r = uj

    T = np.zeros((4,4))

    T[0,0] = cos(theta)
    T[0,1] = -sin(theta)
    T[0,2] = 0
    T[0,3] = a

    T[1,0] = cos(alpha) * sin(theta)
    T[1,1] = cos(alpha) * cos(theta)
    T[1,2] = -sin(alpha)
    T[1,3] = -r * sin(alpha)

    T[2,0] = sin(alpha) * sin(theta)
    T[2,1] = sin(alpha) * cos(theta)
    T[2,2] = cos(alpha)
    T[2,3] = r * cos(alpha)

    T[3,0] = 0
    T[3,1] = 0
    T[3,2] = 0
    T[3,3] = 1

    return T


def kinematics(joint_pos):
    """
    Calcule la cinématique directe du robot et la Jacobienne.

    Paramètres :
        - joint_pos (array): Positions des joints 6x1.

    Retourne :
        - T06 (array): Matrice de transformation totale 4x4.
        - J (array): Jacobienne 6x6.
        - x_actuel (array): Position actuelle de l'effecteur 3x1.
    """
    # ********************************

    # Le pi/2 des joints 2 et 4 sont causé par CoppeliaSim (a retirer sur le vrai robot)
    ui = np.zeros((6, 4))
    ui[0] = [0,       0,      joint_pos[0,0]       ,      r1]
    ui[1] = [pi/2,    0,      joint_pos[1,0] - pi/2,      r2]
    ui[2] = [0,       -a2,    joint_pos[2,0]       ,      0]
    ui[3] = [0,       -a3,    joint_pos[3,0]       ,      0]
    ui[4] = [pi/2,    0,      joint_pos[4,0] - pi/2,      r5]
    ui[5] = [-pi/2,   0,      joint_pos[5,0]       ,      0]


    T = np.zeros(6, dtype=object)

    # T06 = np.eye(4)

    # for u in ui:
    #     T06 = T06 @ Matrix_Transformation(u)
    #     T06 = np.round(T06, decimals=4)

    for i in range(6):
        T[i] = Matrix_Transformation(ui[i])

    # print(T)

    T_product = np.zeros(6, dtype=object)

    T_product[0] = T[0]
    for i in range(1, len(T)):
        T_product[i] = T_product[i-1] @ T[i]
        # T_product[i] = np.dot(T_product[i-1], T[i])
    #     print(T_product[i])

    T06 = T_product[5]
    
    O67 = np.array([
        [0],
        [0],
        [r7],
        [1]
    ])

    x_actuel = T06 @ O67

    #################################
    #### Calcul de la Jacobienne ####
    #################################
    J06 = np.zeros((6,6))

    for i in range(6):
        J06[:3,i] = np.cross(T_product[i][:3,2] , (T_product[5][:3,3] - T_product[i][:3,3]))
        J06[3:,i] = T_product[i][:3,2]


    R06 = T06[:3,:3]
    D = np.array([
        [0              , r7 * R06[2, 2] , -r7 * R06[1, 2]],
        [-r7 * R06[2, 2], 0              , r7 * R06[0, 2]],
        [r7 * R06[1, 2] , -r7 * R06[0, 2], 0]
    ])

    C = np.eye(3)
    J = np.block([
        [np.identity(3),  D],
        [np.zeros((3,3)), C]]) @ J06

    return T06, J, x_actuel[:3, 0]


# DEBUG KINEMATICS
# T06,J,X = kinematics(np.zeros((6,1)))
# np.set_printoptions(precision=4, suppress=True)
# print(T06)
# print(J)
# print(X)



def Trajectoire(t, tf, x_init, x_final, R_init, R_final):
    """
    Génère la trajectoire désirée pour l'effecteur.

    Paramètres :
        - t (float): Temps actuel.
        - tf (float): Temps final.
        - x_init (array): Position initiale 3x1.
        - x_final (array): Position finale  3x1.
        - R_init (array): Rotation initiale 3x3.
        - R_final (array): Rotation finale  3x3.

    Retourne :
        - x_desire (array): Position désirée 3x1.
        - xp_desire (array): Vitesse désirée 3x1.
        - R_desire (array): Rotation désirée 3x3.
        - theta (float): Angle de rotation.
        - u (array): Axe de rotation.
        - r_point (float): Vitesse normalisée.
    """
    # ********************************
    r       = 10 * (t / tf)**3 - 15 * (t / tf)**4 + 6 * (t / tf)**5 
    r_point = 30 * (t**2 / tf**3) - 60 * (t**3 / tf**4) + 30 * (t**4 / tf**5)

    x_desire = x_init + r * (x_final - x_init)
    xp_desire = r_point * (x_final - x_init)

    R = R_init.T @ R_final

    cos_theta = (np.trace(R) - 1) / 2
    sin_theta = (sqrt((R[2,1] - R[1,2])**2 + (R[0,2] - R[2,0])**2 + (R[1,0] - R[0,1])**2)) / 2
    theta = atan2(sin_theta, cos_theta)

    # TODO: le u dois aller dans l'init
    u = np.array([
        [1 / (2*sin(theta)) * (R[2,1] - R[1,2])],
        [1 / (2*sin(theta)) * (R[0,2] - R[2,0])],
        [1 / (2*sin(theta)) * (R[1,0] - R[0,1])]
    ])

    # rot = np.array([
    #     [u[0,0]**2 * (1 - cos(theta)), u[0,0]*u[1,0] * (1 - cos(theta)) - u[2,0] * sin(theta), u[0,0]*u[2,0] * (1 - cos(theta)) + u[1,0] * sin(theta)],
    #     [u[0,0]*u[1,0] * (1 - cos(theta)) + u[2,0] * sin(theta), u[1,0]**2 * (1 - cos(theta)) + cos(theta), u[1,0]*u[2,0] * (1 - cos(theta)) - u[0,0] * sin(theta)],
    #     [u[0,0]*u[2,0] * (1 - cos(theta)) - u[1,0] * sin(theta), u[1,0]*u[2,0] * (1 - cos(theta)) + u[0,0] * sin(theta), u[2,0]**2 * (1 - cos(theta)) + cos(theta)]
    # ])
    rot = np.array([
        [u[0, 0]**2 * (1 - cos(r*theta)) + cos(r*theta),                   u[0, 0] * u[1, 0] * (1 - cos(r*theta)) - u[2, 0] * sin(r*theta),  u[0, 0] * u[2, 0] * (1 - cos(r*theta)) + u[1, 0] * sin(r*theta)],
        [u[0, 0] * u[1, 0] * (1 - cos(r*theta)) + u[2, 0] * sin(r*theta),  u[1, 0]**2 * (1 - cos(r*theta)) + cos(r*theta),                   u[1, 0] * u[2, 0] * (1 - cos(r*theta)) - u[0, 0] * sin(r*theta)],
        [u[0, 0] * u[2, 0] * (1 - cos(r*theta)) - u[1, 0] * sin(r*theta),  u[1, 0] * u[2, 0] * (1 - cos(r*theta)) + u[0, 0] * sin(r*theta),  u[2, 0]**2 * (1 - cos(r*theta)) + cos(r*theta)]
    ])

    R_desire = R_init @ rot

    # print(f"t: {t}, r: {r}, x desire: {x_desire}, x actuel: {x_actuel}")

    return x_desire, xp_desire, R_desire, theta, u, r_point


# =====================================================================
# Calcul de l'erreur entre la position actuelle et la position désirée
def Erreur(x_desire, x_actuel, R_desire, R_actuel):
    """
    Calcule les erreurs de position et d'orientation.

    Paramètres :
        - x_desire (array): Position désirée 3x1.
        - x_actuel (array): Position actuelle 3x1.
        - R_desire (array): Rotation désirée 3x3.
        - R_actuel (array): Rotation actuelle 3x3.

    Retourne :
        - e_position (array): Erreur de position 3x1.
        - e_orientation (array): Erreur d'orientation 3x1.
        - L (array): Matrice d'orientation.
    """
    # ********************************
    e_position = x_desire - x_actuel
    e_orientation = 1/2 * (np.cross(R_actuel[:, 0], R_desire[:, 0]) +
                           np.cross(R_actuel[:, 1], R_desire[:, 1]) +
                           np.cross(R_actuel[:, 2], R_desire[:, 2]))
    L = -1/2 * (S(R_desire[:, 0]) @ S(R_actuel[:, 0]) +
                S(R_desire[:, 1]) @ S(R_actuel[:, 1]) +
                S(R_desire[:, 2]) @ S(R_actuel[:, 2]))
    
    # print(f"Erreur position: {e_position}, Erreur Orientation: {e_orientation}, L: {L}")

    return e_position, e_orientation, L

# Générateur de matrice antisymétrique
def S(matrice):
    """
    Génère une matrice antisymétrique.

    Paramètres :
        - matrice (array): Vecteur 3x1.

    Retourne :
        - N (array): Matrice antisymétrique 3x3.
    """
    # ********************************
    N = np.array([
        [0          , -matrice[2], matrice[1]],
        [matrice[2] , 0          , -matrice[0]],
        [-matrice[1], matrice[0] , 0]
    ])
    return N


# =====================================================================
# Contrôleur de position et d'orientation
def Controleur(xp_desire, R_init, J, r_point, u, theta, e_position, e_orientation, L):
    """
    Calcule les vitesses articulaires désirées via un contrôleur.

    Paramètres :
        - xp_desire (array): Vitesse cartésienne désirée 3x1.
        - R_init (array): Rotation initiale 3x3.
        - J (array): Jacobienne 6x6.
        - r_point (float): Vitesse normalisée.
        - u (array): Axe de rotation.
        - theta (float): Angle de rotation.
        - e_position (array): Erreur de position 3x1.
        - e_orientation (array): Erreur d'orientation 3x1.
        - L (array): Matrice d'orientation.

    Retourne :
        - qp_desire (array): Vitesse articulaire désirée 6x1.
    """
    # ********************************
    wd = R_init @ (r_point * theta * u)

    # J_etoile = np.linalg.pinv(J)
    J_etoile = J.T @ np.linalg.inv(J @ J.T + k**2 * np.identity(6))

    # Calcul du vecteur de commande des moteurs
    qp_desire = J_etoile @ np.vstack([xp_desire[:3].reshape(3, 1) + (Kp @ e_position).reshape(3, 1),
                                    np.linalg.inv(L) @ (L.T @ wd.reshape(3, 1) + (Ko @ e_orientation).reshape(3, 1))
    ])

    return qp_desire