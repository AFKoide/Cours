import math
import numpy as np
import matplotlib.pyplot as plt
import sympy as sp
import tabulate as tabulate

np.set_printoptions(precision=4, suppress=True)

# Raccourci des noms des fonctions trigonométriques et de pi
cos = math.cos
atan2 = math.atan2
sin = math.sin
pi = math.pi
sqrt = math.sqrt

# Déclaration des variables globales
a2 = 0.612
a3 = 0.5723
r1 = 0.1273
r2 = 0.163941
r5 = 0.1157
r7 = 0.0922

# Initialisation de la matrice ui (6x6) avec des zéros
ui = np.zeros([6, 6])

# Fonction de transformation de matrice homogène 4x4
def Matrix_Transformation(ui_row):
    alpha, a, theta, r = ui_row
#    print(alpha, a, theta, r)

    T = np.zeros([4, 4])
    
    T[0, 0] = cos(theta)
    T[0, 1] = -sin(theta)
    T[0, 2] = 0
    T[0, 3] = a
    
    T[1, 0] = cos(alpha) * sin(theta)
    T[1, 1] = cos(alpha) * cos(theta)
    T[1, 2] = -sin(alpha)
    T[1, 3] = -r * sin(alpha)
    
    T[2, 0] = sin(alpha) * sin(theta)
    T[2, 1] = sin(alpha) * cos(theta)
    T[2, 2] = cos(alpha)
    T[2, 3] = r * cos(alpha)
    
    T[3, 0] = 0
    T[3, 1] = 0
    T[3, 2] = 0
    T[3, 3] = 1

    return T


def kinematics(joint_pos):
    # Remplissage du tableau ui
    ui[0] = [1, 0, 0,       0,      joint_pos[0],      r1]
    ui[1] = [2, 0, pi/2,    0,      joint_pos[1],      r2]
    ui[2] = [3, 0, 0,       -a2,    joint_pos[2],      0]
    ui[3] = [4, 0, 0,       -a3,    joint_pos[3],      0]
    ui[4] = [5, 0, pi/2,    0,      joint_pos[4],      r5]
    ui[5] = [6, 0, -pi/2,   0,      joint_pos[5],      0]

    # Affichage du tableau ui
    print("Tableau ui :")
    print(ui)
    print("\n\n\n")



# =================================================================
    # Calcul des matrices de transformation pour chaque ligne de ui
    print("Calcul et affichage des matrices de transformation")
    Transforme = np.zeros(6, dtype=object)
    for i in range(6):
        Transforme[i] = Matrix_Transformation(ui[i, 2:6])

    # Affichage des matrices de transformation
    for i, T in enumerate(Transforme):
        print(f"Matrice de transformation T{i+1} :")
        print(T)
        print()  # Insère une ligne vide
    print("\n\n\n")



# =================================================================
    # Calcul et affichage du produit des matrices de transformation
    print("Calcul et affichage du produit des matrices de transformation")
    T_product = np.zeros(6, dtype=object)
    for i in range(1, len(Transforme)):  # Commence à 1 pour éviter n=0
        if(i == 1):
            T_product[0] = Transforme[0]
            print(f"Matrice {0}T{i}")
            print(T_product[0])
            print()

        # Calcul du produit de la matrice actuelle avec la matrice précédente
        T_product[i] = np.dot(T_product[i-1], Transforme[i])
        
        # Affichage du résultat
        print(f"Produit des matrices {0}T{i+1}:")
        print(T_product[i])
        print()  # Ligne vide pour la lisibilité
    print("\n\n\n")


# =================================================================
    # Calcul et affichage de la Jacobienne
    print("Calcul de la Jacobienne")
    Jn = np.zeros([6,6])

    z = np.zeros((6, 3))
    p = np.zeros((6, 3))

    for i in range(6):
        z[i] = T_product[i][:3, 2]
        p[i] = T_product[i][:3, 3]


    for j in range(6):
        Jn[:3, j]= np.cross(z[j, :], p[5, :] - p[j, :])

        Jn[3:, j]= z[j]

    print("Jn =", Jn)
    print("\n\n")
    
    R06 = np.zeros([3,3])
    R06 = T_product[5][0:3, 0:3]
    D = np.array([
        [0                                , 0 * R06[2, 0] + r7 * R06[2, 2]      , -(0 * R06[1, 0] + r7 * R06[1, 2])],
        [-(0 * R06[2, 0] + r7 * R06[2, 2]), 0                                   , 0 * R06[0, 0] + r7 * R06[0, 2]],
        [0 * R06[1, 0] + r7 * R06[1, 2]   , -(0 * R06[0, 0] + r7 * R06[0, 2])   , 0]
    ])
    
    O06 = np.array([0, 0, r7]).T

    print("Jn:",Jn)

    Presque_J = np.block([
        [np.identity(3), D],
        [np.zeros([3,3]), np.identity(3)]
    ])

    J = Presque_J @ Jn

    print("J =", J)
    print("\n\n\n")


    return -1


def update(t):
    
    if t % dt == 0:
        # Calcul des nouvelles positions et orientations
        xd, xd_point, rd_point, Rd, theta, u = Generateur_Trajectoire(xi, xf, Ri, Rf, tf, t)
        
        # Calcul de l'erreur
        ep, e0, L = Erreur(xd, xe, Rd, Re)

        # Commande des moteurs
        qp= Correcteur_et_Controle(R06, rd_point, theta, u, J, xd_point, e0, ep, L)



def Generateur_Trajectoire(xf, xi, t, tf, Ri, Rf):
    ud = np.zeros(3)
    D  = np.zeros(3)
    R  = np.zeros([3, 3])


    D = xf - xi 
    r = 10*(t/tf)**3 - 15*(t/tf)**4 + 6*(t/tf)**5
    xd = xi + r*D

    R = Ri.T @ Rf
    cos_theta = (np.trace(R) - 1) / 2
    sin_theta = sqrt(1 - cos_theta**2)
    theta = atan2(sin_theta, cos_theta)

    if sin_theta == 0:  # No rotation
        ux, uy, uz = 1, 0, 0
    else:
        ux = (R[2, 1] - R[1, 2]) / (2 * sin_theta)
        uy = (R[0, 2] - R[2, 0]) / (2 * sin_theta)
        uz = (R[1, 0] - R[0, 1]) / (2 * sin_theta)
    ud = ux, uy, uz

    rot = np.array(
        [ux**2 * (1-cos(r*theta)) , ux*uy * (1-cos(r*theta)) - uz*sin(r*theta) , ux*uz * (1-cos(r*theta)) + uy*sin(r*theta)],
        [ux*uy * (1-cos(r*theta)) + uz*sin(r*theta) , uy**2 * (1-cos(theta)) + cos(r*theta) , uy*uz * (1-cos(r*theta)) - ux*sin(r*theta)],
        [ux*uz * (1-cos(r*theta)) - uy*sin(r*theta) , uy*uz * (1-cos(r*theta)) + ux*sin(r*theta) , uz**2 * (1-cos(r*theta)) + cos(r*theta)]
    )

    Rd = Ri @ rot

    rd_point = 3*10*(t**2/tf**3) - 4*15*(t**3/tf**4) + 5*6*(t**4/tf**5)
    xd_point = rd_point * D

    return xd, xd_point, rd_point, Rd, theta, ud



def Erreur(xd, xe, Rd, Re):
    # Re est la matrice de rotation actuelle
    # xe sont les coodonnées actuelles
    ep = xd - xe
    e0 = 1/2 * (np.cross(Re[:, 0], Rd[:, 0]) + np.cross(Re[:, 1], Rd[:, 1]) + np.cross(Re[:, 2], Rd[:, 2]))
    L = -1/2 * (S(Rd[:, 0])@S(Re[:, 0]) + S(Re[:,1]@S(Rd[:,1])) + S(Re[:,2]@S(Rd[:,2])))
    
    return ep, e0, L
# Le produit vectoriel est nul quand les deux axes sont parallèles.

def S(matrice):
    N = np.array([
        [ 0, -matrice[2], matrice[1]],
        [matrice[2], 0, -matrice[0]],
        [-matrice[1], matrice[0], 0]
    ])

    return N



def Correcteur_et_Controle(Ri, r_point, theta, u, J, xd_point, e0, ep, L):
    k = 0.01
    Kp = 1
    K0 = 1

    wd = Ri * r_point*theta*u
    J_etoile = J.T @ np.linalg.inv(J@J.T + k**2 @ np.identity(6))

    qp = J_etoile @ np.array([  [xd_point + Kp*ep],
                                [np.linalg.inv(L) * (L.T*wd + K0*e0)]])


    return qp
# Si det(J) = 0, alors on a une singularité (moteur ont une vitesse infini). 
# On va utiliser J* a la place. A la place d'avoir une vitesse infini, J* va calculer un J pour theta2 tendant vers 0 (mais pas égal à 0).



kinematics(np.zeros(6))