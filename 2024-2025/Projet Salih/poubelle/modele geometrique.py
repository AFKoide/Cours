import numpy as np
import matplotlib.pyplot as plt
from numpy import cos, sin, sqrt, arcsin, arctan2, radians

# np.set_printoptions(precision=2, suppress=True)  # 2 décimales, sans notation scientifique


# ----------------------------------------------------------------
# Définitions du robot.
## Angles
theta10 = 20
theta20 = 20

## Distances
O_A1    = 0.0           # Longueur entre le centre du repère et le point A1
O_A2    = 0.0           # Longueur entre le centre du repère et le point A2
O_T     = 0.0           # Longueur entre le centre du repère et le point T
A1_B1   = 72.648        # Longueur entre le point A1 ('origine' du bras) et B1 (première pliure du bras).
A2_B2   = 84.655        # Longueur entre le point A2 et B2.
A1_A3   = 0.0           # Longueur entre le point A1 et A3. (Valeur temporaire, je sais pas comment la calculer)


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
    beta1, beta2 = radians(beta[0]), radians(beta[1])
    T_norm = O_T            # La norme de T est la distance entre le centre du repère et le point T (la distance est fixe)


    # ------------------------------------------------------------
    # Calcul de la position de T dans le repère global
    A1_T = 4 * A1_B1 * cos(beta1 / 2)
    A2_T = 4 * A2_B2 * cos(beta2 / 2)

    theta1 = 2 * arcsin(A1_T / 2 * T_norm)
    theta2 = 2 * arcsin(A2_T / 2 * T_norm)

    Tx = A1_T * sin(theta1 + theta10)
    Tz = A1_T * cos(theta1 + theta10)
    Ty = sqrt(A1_T**2 - Tx**2 - Tz**2)  # Hypothèse de projection perpendiculaire


    T = np.array([[Tx],
                  [Ty],
                  [Tz]])


    # ------------------------------------------------------------
    # Calcul de l'orientation de T dans le repère global 
    Tμ = abs(arctan2(T_norm * cos(theta2 + theta10), 
                     sqrt(-T_norm**2 + (T_norm * sin(theta1 + theta10))**2 + (T_norm * sin(theta2 + theta10))**2)))

    Tν = abs(arctan2(sqrt(-T_norm**2 + (T_norm * sin(theta1 + theta10))**2 + (T_norm * sin(theta2 + theta10))**2), 
                     T_norm * cos(theta1 + theta10)))

    O = np.array([[Tμ],
                  [Tν],
                  [0]])


    # ------------------------------------------------------------
    # Résultats
    return T, O

T, O = geometric_model([0,0])

print(f"Position: {T.flatten()}, Orientation: {O.flatten()}")



# //////////////////////////////////
def static_model(T):
    """
    Description:
    Calcul le modèle statique du robot.

    1. On cherche le repère trig, qui est le repère xOy avec une rotation de theta10.
    2. On calcule Vx et Vy, les coordonnées de T sur le repère trig.
    3. On cherche la position de A3 : on va calculer deux cercles, l'un avec le point A1 comme centre, et un autre avec T comme centre, et avec comme rayon T_A1 / 2 pour les deux cercles.
       Le point d'intersection des deux cercles sera le point A3.
    4. On cherche la position de A4 et A2 : encore, on cherche les points d'intersection des cercles ayant comme rayon A1_A2 au centre de A1 et de A3.
       Les points d'intersection seront A4 et A2.

    Parameters:
    T (numpy array): Coordonnées de l'organe terminal T dans le repère global.

    Returns:
    dict: Coordonnées des points A1, A2, A3 et A4 dans les repères global et trig.
    """
    # ------------------------------------------------------------
    # 1. Repère TRIG (Rotation de theta10 autour de l'axe y)
    rotation_matrix = np.array([
        [cos(theta10), 0, sin(theta10)],
        [0, 1, 0],
        [-sin(theta10), 0, cos(theta10)]
    ])

    # A1 dans le repère global et trig
    A1_global = np.array([O_A1, 0, 0])
    A1_trig = np.dot(rotation_matrix, A1_global)
    print(f"Repere global: {A1_global}, Repère TRIG: {A1_trig}")

    # ------------------------------------------------------------
    # 2. Projection de T dans le repère trig
    T_trig = np.dot(rotation_matrix, T.flatten())

    # ------------------------------------------------------------
    # 3. Calcul de A3 (Intersection des cercles)
    A3_candidates = circle_intersection(A1_trig[0], A1_trig[2], A1_A3, T_trig[0], T_trig[2], A1_A3)
    A3_trig = A3_candidates[0] if len(A3_candidates) > 0 else None
    if A3_trig is None:
        raise ValueError("Aucune intersection trouvée pour A3")

    # Conversion de A3 dans le repère global
    A3_global = np.dot(rotation_matrix.T, np.array([A3_trig[0], 0, A3_trig[1]]))

    # ------------------------------------------------------------
    # 4. Calcul de A2 et A4 (Intersection des cercles)
    A2_A4_candidates = circle_intersection(A1_trig[0], A1_trig[2], A1_B1, A3_trig[0], A3_trig[1], A1_B1)
    A2_trig = A2_A4_candidates[0] if len(A2_A4_candidates) > 0 else None
    A4_trig = A2_A4_candidates[1] if len(A2_A4_candidates) > 1 else None
    if A2_trig is None or A4_trig is None:
        raise ValueError("Aucune intersection trouvée pour A2 ou A4")

    # Conversion de A2 et A4 dans le repère global
    A2_global = np.dot(rotation_matrix.T, np.array([A2_trig[0], 0, A2_trig[1]]))
    A4_global = np.dot(rotation_matrix.T, np.array([A4_trig[0], 0, A4_trig[1]]))

    # ------------------------------------------------------------
    return {
        "A1_global": A1_global,
        "A1_trig": A1_trig,
        "A3_global": A3_global,
        "A3_trig": A3_trig,
        "A2_global": A2_global,
        "A2_trig": A2_trig,
        "A4_global": A4_global,
        "A4_trig": A4_trig
    }

def circle_intersection(x0, y0, r0, x1, y1, r1):
    """
    Trouve les points d'intersection de deux cercles donnés leurs centres et rayons.

    Parameters:
    x0, y0 : float : Coordonnées du centre du premier cercle.
    r0 : float : Rayon du premier cercle.
    x1, y1 : float : Coordonnées du centre du second cercle.
    r1 : float : Rayon du second cercle.

    Returns:
    list : Liste des points d'intersection [(x1, y1), (x2, y2)]. Peut être vide si aucun point d'intersection.
    """
    # Distance entre les deux centres
    d = sqrt((x1 - x0)**2 + (y1 - y0)**2)

    # Pas d'intersection ou une sphère contient l'autre
    if d > r0 + r1 or d < abs(r0 - r1) or d == 0:
        return []

    # Calcul des points d'intersection
    a = (r0**2 - r1**2 + d**2) / (2 * d)
    h = sqrt(r0**2 - a**2)

    # Point intermédiaire sur la droite joignant les centres
    x2 = x0 + a * (x1 - x0) / d
    y2 = y0 + a * (y1 - y0) / d

    # Points d'intersection
    inter1 = (x2 + h * (y1 - y0) / d, y2 - h * (x1 - x0) / d)
    inter2 = (x2 - h * (y1 - y0) / d, y2 + h * (x1 - x0) / d)

    return [inter1, inter2]



# Exemple d'utilisation
data = static_model(T)
for key, value in data.items():
    print(f"{key}: {value}")