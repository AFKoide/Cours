import math
import numpy as np
import matplotlib.pyplot as plt
import time  # Import pour récupérer le temps système

# Configuration d'affichage
# np.set_printoptions(precision=4, suppress=True)

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

Ko = 3 * np.identity(3)
Kp = 3 * np.identity(3)
k  = 0.1



# Matrice ui (6x6) initialisée à zéro pour stocker les paramètres de transformation DH
ui = np.zeros([6, 6])

# =====================================================================
# Fonction pour calculer une matrice de transformation homogène 4x4
def Matrix_Transformation(ui_row):
    alpha, a, theta, r = ui_row

    T = np.zeros([4, 4])
    
    # Remplissage de la matrice de transformation homogène
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

# Fonction pour calculer les matrices cinématiques (joint_pos: positions des jointures)
def kinematics(joint_pos):
    # Remplissage du tableau ui avec les angles et distances des articulations
    ui[0] = [1, 0, 0,       0,      joint_pos[0,0],      r1]
    ui[1] = [2, 0, pi/2,    0,      joint_pos[1,0],      r2]
    ui[2] = [3, 0, 0,       -a2,    joint_pos[2,0],      0]
    ui[3] = [4, 0, 0,       -a3,    joint_pos[3,0],      0]
    ui[4] = [5, 0, pi/2,    0,      joint_pos[4,0],      r5]
    ui[5] = [6, 0, -pi/2,   0,      joint_pos[5,0],      0]


    # Calcul des matrices de transformation pour chaque articulation
    Transforme = np.zeros(6, dtype=object)
    for i in range(6):
        Transforme[i] = Matrix_Transformation(ui[i, 2:6])  # On ne prend que les 4 derniers éléments de chaque ligne

    # Produit des matrices de transformation
    T_product = np.zeros(6, dtype=object)
    T_product[0] = Transforme[0]
    for i in range(1, len(Transforme)):
        T_product[i] = np.dot(T_product[i-1], Transforme[i])


# -------------
    # Calcul de la Jacobienne
    Jn = np.zeros([6, 6])
    z = np.zeros((6, 3))
    p = np.zeros((6, 3))

    for i in range(6):
        z[i] = T_product[i][:3, 2]  # Axes z des matrices de transformation
        p[i] = T_product[i][:3, 3]  # Points p des matrices de transformation

    for j in range(6):
        Jn[:3, j] = np.cross(z[j], p[5] - p[j])  # Partie linéaire
        Jn[3:, j] = z[j]  # Partie angulaire

    # Extraction de la matrice de rotation de la dernière transformation
    R06 = T_product[5][:3, :3]

    # Calcul du correcteur Jacobien étendu avec les dimensions globales
    D = np.array([
        [0                                , 0 * R06[2, 0] + r7 * R06[2, 2]      , -(0 * R06[1, 0] + r7 * R06[1, 2])],
        [-(0 * R06[2, 0] + r7 * R06[2, 2]), 0                                   , 0 * R06[0, 0] + r7 * R06[0, 2]],
        [0 * R06[1, 0] + r7 * R06[1, 2]   , -(0 * R06[0, 0] + r7 * R06[0, 2])   , 0]
    ])
    Presque_J = np.block([
        [np.identity(3), D],
        [np.zeros([3, 3]), np.identity(3)]
    ])

    J = Presque_J @ Jn

    print(Transforme[5])
    print(J)

    return Transforme[5], J



# =====================================================================
# Fonction pour calculer la trajectoire
def Generateur_Trajectoire(xf, xi, Ri, Rf, tf, t):
    # Calcul du point de la trajectoire
    D = xf - xi
    r = 10 * (t / tf)**3 - 15 * (t / tf)**4 + 6 * (t / tf)**5 
    xd = xi + r * D

    # Calcul du vecteur vitesse
    rd_point = 30 * (t**2 / tf**3) - 60 * (t**3 / tf**4) + 30 * (t**4 / tf**5)
    xd_point = rd_point * D

    # Rotation désirée
    R = Ri.T @ Rf

    cos_theta = (np.trace(R) - 1) / 2
    # sin_theta = sqrt(1 - cos_theta**2) / 2
    sin_theta = (sqrt((R[2,1] - R[1,2])**2 + (R[0,2] - R[2,0])**2 + (R[1,0] - R[0,1])**2)) / 2
    theta = atan2(sin_theta, cos_theta)

    ux = 1 / (2*sin(theta)) * (R[2,1] - R[1,2])
    uy = 1 / (2*sin(theta)) * (R[0,2] - R[2,0])
    uz = 1 / (2*sin(theta)) * (R[1,0] - R[0,1])
    
    # Matrice de rotation
    rot = np.array([
        [ux**2 * (1 - cos(theta)), ux*uy * (1 - cos(theta)) - uz * sin(theta), ux*uz * (1 - cos(theta)) + uy * sin(theta)],
        [ux*uy * (1 - cos(theta)) + uz * sin(theta), uy**2 * (1 - cos(theta)) + cos(theta), uy*uz * (1 - cos(theta)) - ux * sin(theta)],
        [ux*uz * (1 - cos(theta)) - uy * sin(theta), uy*uz * (1 - cos(theta)) + ux * sin(theta), uz**2 * (1 - cos(theta)) + cos(theta)]
    ])

    Rd = Ri @ rot


    print(f"t: {t}, r: {r}, xd: {xd}, x actuel: {xi}")
    return xd, xd_point, rd_point, Rd, theta, ux, uy, uz


# =====================================================================
# Fonction pour calculer les erreurs
def Erreur(xd, xe, Rd, Re):
    # Calcul des erreurs
    ep = xd - xe
    e0 = 1/2 * (np.cross(Re[:, 0], Rd[:, 0]) + np.cross(Re[:, 1], Rd[:, 1]) + np.cross(Re[:, 2], Rd[:, 2]))
    L = -1/2 * (S(Rd[:, 0]) @ S(Re[:, 0]) + S(Rd[:, 1]) @ S(Re[:, 1]) + S(Rd[:, 2]) @ S(Re[:, 2]))
    
    return ep, e0, L

def S(matrice):
    N = np.array([
        [0          , -matrice[2], matrice[1]],
        [matrice[2] , 0          , -matrice[0]],
        [-matrice[1], matrice[0] , 0]
    ])
    return N


# =====================================================================
# Fonction pour la commande et le contrôle des moteurs
def Correcteur_et_Controle(Ri, r_point, theta, u, J, xd_point, eo, ep, L):
    # Calcul de la vitesse désirée
    wd = Ri @ (r_point * theta * u) #     wd = Ri @ (r_point * theta * u) 

    # Calcul de la pseudo-inverse de la Jacobienne
    J_etoile = J.T @ np.linalg.inv(J @ J.T + k**2 * np.identity(6))

    # Calcul du vecteur de commande des moteurs
    qp = J_etoile @ np.vstack([xd_point[:3].reshape(3, 1) + (Kp @ ep).reshape(3, 1),
                              np.linalg.inv(L) @ (L.T @ wd.reshape(3, 1) + (Ko @ eo).reshape(3, 1))
    ])



    return qp.reshape((6,1))


# =====================================================================
# Fonction d'initialisation et boucle de mise à jour
def init():
    global tf, xf, Rf, start_time, q, q_graph, t_graph, x_graph
    start_time = time.time()  # Récupération du temps de démarrage
    tf = 10  # Temps final de la simulation

    xf = np.array([-0.254, 0.266, 0.690])

    q = np.array([0.0, pi/4, pi/4, pi/4, pi/4, 0.0]) # Position initiale
    Rf = np.identity(3)  # Matrice de rotation finale

    q_graph = np.empty((6,1))
    t_graph = np.empty((1,1))
    x_graph = np.empty((3,1))
    update()


    Graphique()




def update():
    global q, q_graph, t_graph, x_graph, Rf, tf
    i=0
    qd = q.reshape((6,1))

    T06, J = kinematics(qd)  # Utilisation de positions de jointure fictives (zeros)
    xa = T06[:3,3]


    while True:
        print("\nN°:",i)
        current_time = time.time()
        t = current_time - start_time  # Temps écoulé depuis le démarrage

        T06, J = kinematics(qd)  # Utilisation de positions de jointure fictives (zeros)


        if t > tf:
            print("Fin de la simulation!")
            return False  # Fin de la simulation

        # Appel à kinematics pour obtenir R06 et la Jacobienne J

        T06, J = kinematics(qd)  # Utilisation de positions de jointure fictives (zeros)
        print(f"T06: {T06}, \nq: {qd}\n")

        # Génération de la trajectoire
        xd, xd_point, rd_point, Rd, theta, ux, uy, uz = Generateur_Trajectoire(xf, xa, T06[:3, :3], Rf, tf, t)
        


        # Calcul de l'erreur
        ep, e0, L = Erreur(xd[:3], xa, Rd, T06[:3, :3])
        
        # Commande des moteurs avec reshaping pour s'assurer que qp est de dimension (6,1)
        qp = Correcteur_et_Controle(T06[:3, :3], rd_point, theta, np.array([ux, uy, uz]), J, xd_point, e0, ep, L)
        
        

        if t_graph.size != 0:
            last_t = t_graph[-1][0] if t_graph.ndim > 1 else t_graph[-1]
            q_last = q_graph[:,-1].reshape(6,1)
            
            qd = (qp * (t - last_t) + q_last).reshape((6,1))

        xa = xd

        # Mise à jour des graphes
        q_graph = np.hstack([q_graph, qd])
        t_graph = np.hstack([t_graph, np.array([[t]])])  # Reformate `t` en 2D pour `hstack`
        x_graph = np.hstack([x_graph, xd.reshape((-1,1))])

        i+=1
        time.sleep(0.1)



def Graphique():
    global q_graph, x_graph
    
    # Nombre de sous-graphiques (un pour chaque élément de q_graph)
    fig, axs = plt.subplots(1, 1)  # 6 lignes, 1 colonne de subplots

    # Couleurs différentes pour chaque courbe
    couleurs = ['b', 'g', 'r', 'c', 'm', 'y']
    
    for i in range(6):
        plt.plot(q_graph[i,:], color=couleurs[i], label=f'q{i+1}')

    plt.grid("minor")
    
    # Afficher le graphique
    plt.show()












if __name__ == "__main__":
    init()  # Initialisation