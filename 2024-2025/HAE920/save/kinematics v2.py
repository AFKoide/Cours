import math
import numpy as np
import matplotlib.pyplot as plt
import time
import tabulate

np.set_printoptions(precision=4, suppress=True)

# Alias pour les fonctions trigonométriques et constantes
cos = math.cos
atan2 = math.atan2
sin = math.sin
pi = math.pi
sqrt = math.sqrt

# Déclaration des variables globales
a2 = 0.612  # Longueur du lien 2
a3 = 0.5723  # Longueur du lien 3
r1 = 0.1273  # Distance décalée pour la jointure 1
r2 = 0.163941  # Distance décalée pour la jointure 2
r5 = 0.1157  # Distance décalée pour la jointure 5
r7 = 0.0922  # Distance décalée pour la jointure 7




def Matrix_Transformation(uj):
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
    ui = np.zeros((6, 6))
    ui[0] = [1, 0, 0,       0,      joint_pos[0,0],      r1]
    ui[1] = [2, 0, pi/2,    0,      joint_pos[1,0],      r2]
    ui[2] = [3, 0, 0,       -a2,    joint_pos[2,0],      0]
    ui[3] = [4, 0, 0,       -a3,    joint_pos[3,0],      0]
    ui[4] = [5, 0, pi/2,    0,      joint_pos[4,0],      r5]
    ui[5] = [6, 0, -pi/2,   0,      joint_pos[5,0],      0]


    T = np.zeros(6, dtype=object)

    for i in range(6):
        T[i] = Matrix_Transformation(ui[i, 2:6])

    T_product = np.zeros(6, dtype=object)

    T_product[0] = T[0]
    for i in range(1, len(T)):
        T_product[i] = np.dot(T_product[i-1], T[i])

    T06 = T_product[5]
    # print(T06)
    print(T06.shape)

    #################################
    #### Calcul de la Jacobienne ####
    #################################
    J06 = np.zeros((6,6))

    for i in range(6):
        J06[:3,i] = np.cross(T_product[i][:3,2] , T_product[5][:3,3] - T_product[i][:3,3])
        J06[3:,i] = T_product[i][:3,2]


    R06 = T06[:3,:3]
    D = np.array([
        [0                                , 0 * R06[2, 0] + r7 * R06[2, 2]      , -(0 * R06[1, 0] + r7 * R06[1, 2])],
        [-(0 * R06[2, 0] + r7 * R06[2, 2]), 0                                   , 0 * R06[0, 0] + r7 * R06[0, 2]],
        [0 * R06[1, 0] + r7 * R06[1, 2]   , -(0 * R06[0, 0] + r7 * R06[0, 2])   , 0]
    ])

    C = np.identity(3)
    J = np.block([
        [np.identity(3),  D],
        [np.zeros((3,3)), C]]) @ J06

    print(J)

    return T06, J


