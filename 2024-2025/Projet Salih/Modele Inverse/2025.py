import numpy as np
import matplotlib.pyplot as plt
from numpy import cos, sin, sqrt, arcsin, arctan2

np.set_printoptions(precision=2, suppress=True)  # 2 décimales


# ----------------------------------------------------------------
# Définitions des dimensions du robot.
## Angles
theta10 = 20            # Angle entre l'axe x du repère global et l'axe x du bras 1.
theta20 = 20            # Angle entre l'axe x du repère global et l'axe y du bras 2.

beta1 = 125             # Angle d'ouverture du bras 1 (angle entre A1A2 et A1A4)
beta2 = 125             # Angle d'ouverture du bras 2 (angle entre B1B2 et B1B4)
beta = [np.radians(beta1), np.radians(beta2)]


## Distances
O_T     = 122.5         # Longueur entre le centre du repère et l'organe terminal

### Bras 1
O_A1    = 122.5         # Longueur entre le centre du repère et le point A1 (origine du bras 1)
A1_A2   = 73.542        # Longueur entre le point A1 ('origine' du bras) et A2 (première pliure du bras).
A1_A4   = 73.542        # Longueur entre le point A1 et A4 (deuxième pliure du bras).
# A1_A3 a déterminer    # Longueur entre le point A1 et A3 (pliure centrale du bras).
# A1_T  a déterminer    # Longueur entre le point A1 et T (organe terminal).

### Bras 2
O_B1    = 142.5         # Longueur entre le centre du repère et le point B1 (origine du bras 2)
B1_B2   = 85.549        # Longueur entre le point B1 ('origine' du bras) et B2 (première pliure du bras).
B1_B4   = 85.549        # Longueur entre le point B1 et B4 (deuxième pliure du bras).
# B1_B3 a déterminer    # Longueur entre le point B1 et B3 (pliure centrale du bras).
# B1_T  a déterminer    # Longueur entre le point B1 et T (organe terminal).


# Autres paramètres
arrow_length = 10       # Taille des axes du repère

# ------------------------------------------------------------
# 0. Initialisation de l'affichage
def initialize_3d_plot():
    """
    Initialise une figure 3D avec des axes adaptés pour une bonne visibilité.
    Les flèches représentant les axes des repères seront visibles, peu importe la taille des éléments tracés.
    """
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    # Tracer les axes principaux pour le repère global (R0)
    ax.quiver(0, 0, 0, arrow_length, 0, 0, color="red", label="X_global")    # Axe X
    ax.quiver(0, 0, 0, 0, arrow_length, 0, color="green", label="Y_global")  # Axe Y
    ax.quiver(0, 0, 0, 0, 0, arrow_length, color="blue", label="Z_global")   # Axe Z

    ax.scatter(0, 0, 0, color="black", s=100, label="O")  # Point d'origine du repère global


    # Ajustements dynamiques des limites des axes (fonction d'auto-scaling)
    ax.set_box_aspect([1, 1, 1])  # Égalité des proportions entre X, Y et Z


    # Labels des axes
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')

    # Titre par défaut
    plt.title("Affichage 3D des repères et des points")
    
    return fig, ax

fig, ax = initialize_3d_plot()


# ------------------------------------------------------------
# 1. Faire le modele géométrique direct du robot pour déterminer les coordonnées de T dans le repère global.
"""
On a les dimensions du robot, et on veut déterminer les coordonnées de T dans le repère global.
On a l'angle theta10, et l'angle beta1, qui est l'ouverture du bras 1. 
On utilise la loi du parralelogramme pour déterminer la distance entre A1 et T.
Ensuite, on calcul les coordonnées et l'orientation de l'organe terminal dans le repère global.
"""
def geometric_model(beta):
    """
    Description:
    Calcule le modèle géométrique direct (MGD) du robot.

    Parameters:
    beta (list): Liste contenant les deux angles [beta1, beta2] en radians représentant les angles d'ouverture des ciseaux.

    Returns:
    T (numpy array, shape (3, 1)): Coordonnées cartésiennes de la position de T dans le repère global.
    O (numpy array, shape (3, 1)): Orientation de T selon les angles d'orientation μ et ν.
    """
    beta1, beta2 = beta
    T_norm = O_T  # La norme de T est la distance entre le centre du repère et le point T (la distance est fixe)

    R1 = O_A1
    R2 = O_B1


    # ------------------------------------------------------------
    # Calcul de la position de T dans le repère global
    A1_T = 4 * A1_A2 * cos(beta1 / 2)
    B1_T = 4 * B1_B2 * cos(beta2 / 2)

    theta1 = 2 * arcsin(A1_T / (2 * T_norm))
    theta2 = 2 * arcsin(B1_T / (2 * T_norm))

    r1 = T_norm - sin(theta1 + np.radians(theta10))
    r2 = T_norm - sin(theta2 + np.radians(theta20))

    Tx = T_norm * cos(theta1 + np.radians(theta10))
    Ty = T_norm * cos(theta2 + np.radians(theta20))
    Tz = sqrt(- T_norm**2 + r1**2 + r2**2)

    T = np.array([[Tx],
                  [Ty],
                  [Tz]])

    # ------------------------------------------------------------
    # Calcul de l'orientation de T dans le repère global 
    Tμ = abs(arctan2(T_norm * cos(theta2 + np.radians(theta10)),
                     sqrt(-T_norm**2 + (T_norm * sin(theta1 + np.radians(theta10)))**2 + (T_norm * sin(theta2 + np.radians(theta10)))**2)))

    Tν = abs(arctan2(sqrt(-T_norm**2 + (T_norm * sin(theta1 + np.radians(theta10)))**2 + (T_norm * sin(theta2 + np.radians(theta10)))**2),
                     T_norm * cos(theta1 + np.radians(theta10))))

    O = np.array([[Tμ],
                  [Tν],
                  [0]])

    # ------------------------------------------------------------
    # Résultats
    return T, O

T, O = geometric_model(beta)

# Afficher les résultats
print(f"Position de T dans le repère global :\n{T}")
print(f"Orientation de T dans le repère global :\n{O}")


# ------------------------------------------------------------

# ------------------------------------------------------------
# 2. On détermine les coordonées des points de l'articulation


# ------------ Construction du repère trig & Projeté de T ------------
"""
On crée le repère trig. Ce repère est choisi pour :
- x_trig passe par le point A1
- y_trig est perpendiculaire a x, et pour que le point T se trouve sur le plan x_trig y_trig.

Equation donné dans le cours (fig 14) :
x_trig = A1/||A1||
z_trig = T*x_trig / ||T*x_trig||
y_trig = z_trig * x_trig

Avec ca, on fait la matrice de rotation R_O_trig.

Dans l'équation :
- A1 est la coordonnée de A1 dans le repère global. On connait O_A1, et on sait que O_A1 a un angle theta10 entre l'axe x et ce segment. Composant y est nulle.
- T est la coordonnée dans le repère global, obtenue avec le modèle géométrique.

Une fois que l'on a ca, on calcul Vx et Vy, les projetés de T dans le repère trig en utilisant la matrice de rotation R_O_trig.
Normalement, Vz est nul. Pour rappel :
- Vx = T.T * x_trig
- Vy = T.T * y_trig

On aura besoin de Vx et Vy pour le calcul de A3, ainsi que de la coordonnée de A1 dans le repère trig (O_A1, 0)
"""

# Fonction pour calculer le repère trig 
def compute_trig_frame(A1_global, T_global):
    """
    Calcule les axes du repère trigonométrique basé sur les positions de A1 et T dans le repère global.
    
    Parameters:
        A1_global (numpy array): Coordonnées de A1 dans le repère global (3x1 array).
        T_global (numpy array): Coordonnées de T dans le repère global (3x1 array).

    Returns:
        R_O_trig (numpy array): Matrice de rotation (3x3) pour passer du repère global au repère trigonométrique.
    """
    # Calcul des axes du repère trig
    x_trig = A1_global / np.linalg.norm(A1_global)  # Axe x_trig normalisé
    T_cross_x = np.cross(T_global.flatten(), x_trig.flatten())  # Produit vectoriel T * x_trig
    z_trig = T_cross_x / np.linalg.norm(T_cross_x)  # Axe z_trig normalisé
    y_trig = np.cross(z_trig, x_trig)  # Axe y_trig orthogonal

    # Matrice de rotation R_O_trig
    R_O_trig = np.column_stack((x_trig, y_trig, z_trig))

    return R_O_trig

# Fonction pour projeter T dans le repère trigonométrique
def project_T_in_trig_frame(R_O_trig, T_global):
    """
    Calcule les coordonnées de T dans le repère trigonométrique.

    Parameters:
        R_O_trig (numpy array): Matrice de rotation (3x3) du repère global au repère trig.
        T_global (numpy array): Coordonnées de T dans le repère global (3x1 array).

    Returns:
        Vx (float): Coordonnée projetée de T sur l'axe x_trig.
        Vy (float): Coordonnée projetée de T sur l'axe y_trig.
    """
    # Transformation de T dans le repère trig
    T_trig = R_O_trig.T @ T_global  # Produit matriciel pour changer de repère

    Vx = T_trig[0, 0]  # Coordonnée selon x_trig
    Vy = T_trig[1, 0]  # Coordonnée selon y_trig

    return Vx, Vy


# Coordonnées de A1 dans le repère global
A1_global = np.array([[A1_A2 * cos(np.radians(theta10))],
                      [0],
                      [A1_A2 * sin(np.radians(theta10))]])  # O_A1 est sur le plan XZ

# Coordonnées de T dans le repère global (résultat du modèle géométrique)
T_global = T  # Calculé précédemment avec la fonction `geometric_model`

# Calculer le repère trig
R_O_trig = compute_trig_frame(A1_global, T_global)

# Projeter T dans le repère trig
Vx, Vy = project_T_in_trig_frame(R_O_trig, T_global)

# Affichage des résultat
print(f"Coordonnée projetée Vx de T dans le repère trig :\n{Vx}")
print(f"Coordonnée projetée Vy de T dans le repère trig :\n{Vy}")


# ------------ Calcul de A3 ------------
"""
Dans le repère trig, on trace deux cercles :
- Centre A1, rayon A1_A3.
- Centre T, rayon A1_A3.

A1_A3 se calcul avec l'équation de la diagonale d'un parallellogramme.

Une fois les coordonnées de A3 trouvé dans le repère trig, on les remets dans le repère global.
"""
def calculate_A3(A1_trig, T_trig, beta1, R_O_trig):
    """
    Calcule les coordonnées de A3 dans le repère global.
    
    Parameters:
        A1_trig (numpy array): Coordonnées de A1 dans le repère trigonométrique (2x1 array).
        T_trig (numpy array): Coordonnées de T dans le repère trigonométrique (2x1 array).
        beta1 (float): Angle d'ouverture du parallélogramme en radians.
        R_O_trig (numpy array): Matrice de rotation (3x3) du repère global au repère trig.

    Returns:
        A3_global (numpy array): Coordonnées de A3 dans le repère global (3x1 array).
    """
    # Calcul de la distance A1_A3 à partir de l'angle beta1
    A1_A3 = sqrt(A1_A2**2 + A1_A4**2 + 2 * A1_A2 * A1_A4 * cos(beta1))

    # Distance entre A1 et T dans le repère trig
    d = np.linalg.norm(T_trig - A1_trig)

    if d > 2 * A1_A3:
        raise ValueError("Les cercles ne se croisent pas : vérifiez les longueurs ou beta1.")
    if d == 0:
        raise ValueError("Les cercles sont confondus : vérifiez les longueurs ou beta1.")

    # Intersection des cercles
    ## Cercle centré sur A1
    a = (A1_A3**2 - A1_A3**2 + d**2) / (2 * d)

    ## Point à mi-chemin entre A1 et T
    P2 = A1_trig + a * (T_trig - A1_trig) / d

    ## Hauteur perpendiculaire (toujours positive grâce à sqrt)
    h = sqrt(A1_A3**2 - a**2)

    ## Intersection unique, car les cercles ont le même diamètre
    x3 = P2[0, 0] + h * (T_trig[1, 0] - A1_trig[1, 0]) / d
    y3 = P2[1, 0] - h * (T_trig[0, 0] - A1_trig[0, 0]) / d

    A3_trig = np.array([[x3], [y3]])

    ## Convertir A3 dans le repère global
    A3_trig_3D = np.array([[A3_trig[0, 0]], [0], [A3_trig[1, 0]]])  # Ajouter y = 0 pour le repère trig
    A3_global = np.dot(R_O_trig, A3_trig_3D)

    return A3_global



A1_trig = np.array([[A1_global[0]],
                    [A1_global[1]]])
T_trig  = np.array([[Vx],
                    [Vy]])

A3_global = calculate_A3(A1_trig, T_trig, beta1, R_O_trig)

# Affichage des résultats
print(f"Coordonnées de A3 dans le repère global : {A3_global}")


# ------------ Calcul de A2 & A4 ------------
"""
On utiliser le trilateration pour trouver A2 et A4.
Trois sphères :
- Centré sur A3, rayon = A1_A2
- Centré sur A1, rayon = A1_A2
- Centré sur O, rayon = O_A1.

Faudra ensuite discerner A2 de A4. Si l'on trace un segment entre A1 et A3, A2 se trouve à "gauche" de ce segment, et A4 à "droite".
"""
def Trilateration(c1, c2, c3, r1, r2, r3):
    """
    Calcul des coordonnées des points d'intersection de trois sphères par la méthode de Trilateration.
    
    Parameters:
    c1, c2, c3 (numpy array): Coordonnées des centres des sphères.
    r1, r2, r3 (float): Rayons des sphères.

    Returns:
    tuple: Deux points (numpy arrays) correspondant aux intersections possibles.
    """
    # Vector from c1 to c2
    ex = (c2 - c1) / np.linalg.norm(c2 - c1)

    # Distance between c1 and c2
    d = np.linalg.norm(c2 - c1)

    # Project c3 onto the line defined by c1 and c2
    i = np.dot(ex, c3 - c1)
    ey = (c3 - c1 - i * ex) / np.linalg.norm(c3 - c1 - i * ex)

    ez = np.cross(ex, ey)

    # Calculate the x, y coordinates of the circle's center in the plane
    x = (r1**2 - r2**2 + d**2) / (2 * d)
    y = (r1**2 - r3**2 + i**2 + np.linalg.norm(c3 - c1 - i * ex)**2 - x**2) / (2 * np.linalg.norm(c3 - c1 - i * ex))

    # Center of the intersection circle
    p_center = c1 + x * ex + y * ey

    # Radius of the intersection circle
    h = np.sqrt(max(0, r1**2 - x**2 - y**2))

    # Intersection points
    p1 = p_center + h * ez
    p2 = p_center - h * ez

    return p1, p2


A2, A4 = Trilateration(A1_global, A3_global, O, A1_A2, A1_A2, O_A1)





# ------------ Affichage graphique ------------
# Tracer le repère trig

# Tracer les points A1, A3, A2 et A4 dans le repère global


# ------------------------------------------------------------
# 3. Modèle Statique
"""
Voir le cours (page 13).

A faire plus tard.
"""





# ------------------------------------------------------------
# 4. Affichage du robot
plt.show()  # Afficher le graphique
