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


k  = 1
Ko = 1 * np.identity(3)
Kp = 1 * np.identity(3)


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





def Trajectoire(t, tf, x_actuel, x_final, R_actuel, R_final):
    r = 10 * (t / tf)**3 - 15 * (t / tf)**4 + 6 * (t / tf)**5 
    r_point = 30 * (t**2 / tf**3) - 60 * (t**3 / tf**4) + 30 * (t**4 / tf**5)
    
    x_desire = x_actuel + r * (x_final - x_actuel)
    xp_desire = r_point * (x_final - x_actuel)

    R = R_actuel.T @ R_final

    cos_theta = (np.trace(R) - 1) / 2
    sin_theta = (sqrt((R[2,1] - R[1,2])**2 + (R[0,2] - R[2,0])**2 + (R[1,0] - R[0,1])**2)) / 2
    theta = atan2(sin_theta, cos_theta)

    u = np.array([
        [1 / (2*sin(theta)) * (R[2,1] - R[1,2])],
        [1 / (2*sin(theta)) * (R[0,2] - R[2,0])],
        [1 / (2*sin(theta)) * (R[1,0] - R[0,1])]
    ])
    print(u.shape)

    rot = np.array([
        [u[0,0]**2 * (1 - cos(theta)), u[0,0]*u[1,0] * (1 - cos(theta)) - u[2,0] * sin(theta), u[0,0]*u[2,0] * (1 - cos(theta)) + u[1,0] * sin(theta)],
        [u[0,0]*u[1,0] * (1 - cos(theta)) + u[2,0] * sin(theta), u[1,0]**2 * (1 - cos(theta)) + cos(theta), u[1,0]*u[2,0] * (1 - cos(theta)) - u[0,0] * sin(theta)],
        [u[0,0]*u[2,0] * (1 - cos(theta)) - u[1,0] * sin(theta), u[1,0]*u[2,0] * (1 - cos(theta)) + u[0,0] * sin(theta), u[2,0]**2 * (1 - cos(theta)) + cos(theta)]
    ])

    R_desire = R_actuel @ rot

    print(f"t: {t}, r: {r}, x desire: {x_desire}, x actuel: {x_actuel}")

    return x_desire, xp_desire, R_desire, theta, u, r_point



def Erreur(x_desire, x_actuel, R_desire, R_actuel):
    e_position = x_desire - x_actuel
    e_orientation = 1/2 * (np.cross(R_desire[:, 0], R_desire[:, 0]) + np.cross(R_actuel[:, 1], R_desire[:, 1]) + np.cross(R_actuel[:, 2], R_desire[:, 2]))
    L = -1/2 * (S(R_desire[:, 0]) @ S(R_desire[:, 0]) + S(R_desire[:, 1]) @ S(R_actuel[:, 1]) + S(R_desire[:, 2]) @ S(R_actuel[:, 2]))
    
    print(f"Erreur position: {e_position}, Erreur Orientation: {e_orientation}, L: {L}")

    return e_position, e_orientation, L

def S(matrice):
    N = np.array([
        [0          , -matrice[2], matrice[1]],
        [matrice[2] , 0          , -matrice[0]],
        [-matrice[1], matrice[0] , 0]
    ])
    return N




def Controleur(xp_desire, R_actuel, J, r_point, u, theta, e_position, e_orientation, L):
    wd = R_actuel @ (r_point * theta * u)

    # J_etoile = np.linalg.inv(J)
    J_etoile = J.T @ np.linalg.inv(J @ J.T + k**2 * np.identity(6))

    # Calcul du vecteur de commande des moteurs
    qp_desire = J_etoile @ np.vstack([xp_desire[:3].reshape(3, 1) + (Kp @ e_position).reshape(3, 1),
                              np.linalg.inv(L) @ (L.T @ wd.reshape(3, 1) + (Ko @ e_orientation).reshape(3, 1))
    ])

    return qp_desire






def init():
    global t_final, q_init, x_final, R_final
    t_init = time.time()
    t_final = 10
    
    q_init = np.array([0.0, pi/4, pi/4, pi/4, pi/4, 0.0]) # Position initiale
    x_final = np.array([-0.254, 0.266, 0.690])
    R_final = np.identity(3)

    update()

    Affichage()


def update():
    global q_init, x_final, R_final, t_final

    t_actuel_array = np.array([])
    q_array = np.empty((6,0))
    qp_array = np.empty((6,0))
    x_actuel_array = np.empty((3,0))


    q = q_init.reshape((6,1))

    t_init = time.time()
    while True:
        t_actuel = time.time() - t_init

        if t_actuel >= t_final:
            Affichage(t_actuel_array, x_actuel_array, q_array, qp_array)
            print("FIN DE LA SIMULATION!")
            break


        T06, J = kinematics(q)
        x_actuel = T06[:3, 3]  # Position actuelle
        R_actuel = T06[:3, :3]  # Rotation actuelle

        x_desire, xp_desire, R_desire, theta, u, r_point = Trajectoire(t_actuel, t_final, x_actuel, x_final, R_actuel, R_final)


        e_position, e_orientation, L = Erreur(x_desire, x_actuel, R_desire, R_actuel)


        qp_desire = Controleur(xp_desire, R_actuel, J, r_point, u, theta, e_position, e_orientation, L)


        if t_actuel_array.size != 0:
            t_precedent = t_actuel_array[-1][0] if t_actuel_array.ndim > 1 else t_actuel_array[-1]
            q_precedent = q_array[:,-1].reshape(6,1)
            
            q = (qp_desire * (t_actuel - t_precedent) + q_precedent).reshape((6,1))



        print(f"Taille q: {q_array.shape}, Taill x: {x_actuel_array.shape}, Taille t: {t_actuel_array.shape}")

        t_actuel_array = np.hstack((t_actuel_array, np.array([t_actuel])))
        x_actuel_array = np.hstack((x_actuel_array, x_actuel.reshape(3, 1)))
        q_array        = np.hstack((q_array, q.reshape(6, 1)))
        qp_array       = np.hstack((qp_array, qp_desire.reshape(6, 1)))








def Affichage(t_actuel_array, x_actuel_array, q_array, qp_array):
    for i in range(6):
        plt.plot(t_actuel_array, q_array[i,:], label=f'Joint {i+1}')
    plt.xlabel("Temps (s)")
    plt.ylabel("Position des joints (rad)")
    plt.title("Position des joints au cours du temps")
    plt.legend()
    plt.grid(True)

    # Tracer la position cartésienne actuelle en fonction du temps
    plt.figure()
    for i, coord in enumerate(['x', 'y', 'z']):
        plt.plot(t_actuel_array, x_actuel_array[i,:], label=f'Position {coord}')
    plt.xlabel("Temps (s)")
    plt.ylabel("Position cartésienne (m)")
    plt.title("Position cartésienne de l'effecteur au cours du temps")
    plt.legend()
    plt.grid(True)


    plt.figure()
    print(qp_array.shape)
    for i in range(6):
        plt.plot(t_actuel_array, qp_array[i, :], label=f'Velocity Joint {i+1}')
    plt.xlabel("Temps (s)")
    plt.ylabel("Vitesse des joints (rad/s)")
    plt.title("Vitesse des joints au cours du temps")
    plt.legend()
    plt.grid(True)

    plt.show()


    plt.show()


init()



