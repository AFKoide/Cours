import numpy as np
import matplotlib.pyplot as plt
from numpy import cos, sin, sqrt, arcsin, arctan2

np.set_printoptions(precision=2, suppress=True)  # 2 décimales

# ----------------- Paramètres du robot -----------------
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


# ----------------- Fonctions utiles -----------------
def plot_sphere(ax, center, radius, color='b', alpha=0.3):
  """
  Affiche une sphère sur un graphique 3D.
  
  Paramètres:
  ax (Axes3D): Graphique 3D.
  center (numpy array, shape (3,)): Coordonnées du centre de la sphère.
  radius (float): Rayon de la sphère.
  color (str): Couleur de la sphère.
  alpha (float): Transparence de la sphère.
  """
  u, v = np.mgrid[0:2*np.pi:100j, 0:np.pi:50j]
  x = center[0] + radius * np.cos(u) * np.sin(v)
  y = center[1] + radius * np.sin(u) * np.sin(v)
  z = center[2] + radius * np.cos(v)
  ax.plot_surface(x, y, z, color=color, alpha=alpha)

def plot_circle_intersection(centerA, radiusA, centerB, radiusB, ax):
  """
  Affiche le cercle d'intersection entre deux sphères sur un graphique 3D.
  
  Paramètres:
  centerA (numpy array, shape (3,)): Coordonnées du centre de la première sphère.
  radiusA (float): Rayon de la première sphère.
  centerB (numpy array, shape (3,)): Coordonnées du centre de la seconde sphère.
  radiusB (float): Rayon de la seconde sphère.
  ax (Axes3D): Graphique 3D.
  """
  d = np.linalg.norm(np.array(centerA) - np.array(centerB))
  if d > radiusA + radiusB or d < abs(radiusA - radiusB):
      print("No intersection or one sphere is inside the other.")
      return
  
  a = (radiusA**2 - radiusB**2 + d**2) / (2 * d)
  h = np.sqrt(radiusA**2 - a**2)
  
  P = np.array(centerA) + a * (np.array(centerB) - np.array(centerA)) / d
  
  v = np.array(centerB) - np.array(centerA)
  v = v / np.linalg.norm(v)
  
  if v[0] == 0 and v[1] == 0:
      perp = np.array([1, 0, 0])
  else:
      perp = np.array([-v[1], v[0], 0])
  perp = perp / np.linalg.norm(perp)
  w = np.cross(v, perp)
  
  theta = np.linspace(0, 2 * np.pi, 100)
  circle_x = P[0] + h * (perp[0] * np.cos(theta) + w[0] * np.sin(theta))
  circle_y = P[1] + h * (perp[1] * np.cos(theta) + w[1] * np.sin(theta))
  circle_z = P[2] + h * (perp[2] * np.cos(theta) + w[2] * np.sin(theta))
  
  ax.plot(circle_x, circle_y, circle_z, 'k', linewidth=2)



# ----------------- Trilatération -----------------
## Trilatération en 2D
def trilateration_2D(center1, radius1, center2, radius2, center3, radius3, tol=1e-2):
  """
  Calcul de la position d'un point dans le repère 2D à partir de trois positions et trois distances.
  
  Paramètres:
  center1 (numpy array, shape (2,)): Coordonnées du centre du premier cercle.
  radius1 (float): Rayon du premier cercle.
  center2 (numpy array, shape (2,)): Coordonnées du centre du second cercle.
  radius2 (float): Rayon du second cercle.
  center3 (numpy array, shape (2,)): Coordonnées du centre du troisième cercle.
  radius3 (float): Rayon du troisième cercle.

  Retourne:
  point (numpy array, shape (2,)): Coordonnées du point trouvé.
  """
  # ----------------------------------------------------------------
  def circle_intersection(c1, r1, c2, r2):
    d = np.linalg.norm(c2 - c1)
    if d > r1 + r2 or d < abs(r1 - r2):
      return None  # No intersection

    a = (r1**2 - r2**2 + d**2) / (2 * d)
    h = sqrt(r1**2 - a**2)
    p2 = c1 + a * (c2 - c1) / d
    intersection1 = p2 + h * np.array([-(c2[1] - c1[1]), c2[0] - c1[0]]) / d
    intersection2 = p2 - h * np.array([-(c2[1] - c1[1]), c2[0] - c1[0]]) / d
    return intersection1, intersection2
  # ----------------------------------------------------------------

  inter1 = circle_intersection(center1, radius1, center2, radius2)
  inter2 = circle_intersection(center2, radius2, center3, radius3)
  inter3 = circle_intersection(center1, radius1, center3, radius3)

  if None in [inter1, inter2, inter3]:
    raise ValueError("Pas d'intersection commune entre les cercles")

  for point in inter1:
    if np.any(np.all(np.isclose(point, inter2, atol=tol), axis=1)) and np.any(np.all(np.isclose(point, inter3, atol=tol), axis=1)):
      return point
  
  return None  # Pas d'intersection trouvée


## Trilatération en 3D
def trilateration_3D(center1, radius1, center2, radius2, center3, radius3, tol=1e-2):
  """
  Calcul de la position d'un point dans le repère 3D à partir de trois positions et trois distances.
  
  Paramètres:
  center1 (numpy array, shape (3,)): Coordonnées du centre du premier cercle.
  radius1 (float): Rayon du premier cercle.
  center2 (numpy array, shape (3,)): Coordonnées du centre du second cercle.
  radius2 (float): Rayon du second cercle.
  center3 (numpy array, shape (3,)): Coordonnées du centre du troisième cercle.
  radius3 (float): Rayon du troisième cercle.

  Retourne:
  point1 (numpy array, shape (3,)): Coordonnées du premier point trouvé.
  point2 (numpy array, shape (3,)): Coordonnées du second point trouvé.
  """

  A, B, C = np.array(center1), np.array(center2), np.array(center3)

  d12 = np.linalg.norm(B - A)
  d13 = np.linalg.norm(C - A)

  # Vérification si les sphères se touchent
  if d12 > radius1 + radius2 or d12 < abs(radius1 - radius2):
    return None  # Pas d'intersection entre les sphères 1 et 2
  if d13 > radius1 + radius3 or d13 < abs(radius1 - radius3):
    return None  # Pas d'intersection entre les sphères 1 et 3

  # Base orthonormée locale
  ex = (B - A) / d12
  i = np.dot(ex, (C - A))
  ey = (C - A - i * ex) / np.linalg.norm(C - A - i * ex)
  ez = np.cross(ex, ey)

  # Calcul des coordonnées des points de contact sur les sphères 1, 2 et 3
  x = (radius1**2 - radius2**2 + d12**2) / (2 * d12)
  y = (radius1**2 - radius3**2 + d13**2 - 2 * i * x) / (2 * np.dot(ey, C - A))
  z = sqrt(radius1**2 - x**2 - y**2)

  if abs(z) < 0:
    return None  # Pas de solution réelle
  
  # Coordonnées des points d'intersection
  point1 = A + x * ex + y * ey + z * ez
  point2 = A + x * ex + y * ey - z * ez

  return point1, point2



# ----------------- Calcul des coordonnées -----------------
def bras_1(T_global, affichage=False):
  # Coordonnées du point A1 est suivant x
  O_global = np.array([0, 0, 0])
  A1_global = np.array([O_A1 * cos(theta10), 0, O_A1 * sin(theta10)])

  # ----- Point A3 -----
  A1_A3 = sqrt(A1_A2**2 + A1_A4**2 - 2 * A1_A2 * A1_A4 * cos(beta[0]))

  # Repère trig
  x_trig = A1_global / np.linalg.norm(A1_global)
  z_trig = np.cross(T_global.flatten(), x_trig.flatten()) / np.linalg.norm(np.cross(T_global.flatten(), x_trig.flatten()))
  y_trig = np.cross(z_trig, x_trig)

  O_trig = np.array([0, 0])
  A1_trig = np.array([A1_global.T @ x_trig, A1_global.T @ y_trig])
  T_trig = np.array([T_global.T @ x_trig, T_global.T @ y_trig])

  # Trilateration
  A3_trig = trilateration_2D(O_trig, O_A1, A1_trig, A1_A3, T_trig, A1_A3)
  A3_global = O_global + A3_trig[0] * x_trig + A3_trig[1] * y_trig


  # ----- Point A2 & A4 -----
  A2_global, A4_global = trilateration_3D(O_global, O_A1, A1_global, A1_A2, A3_global, A1_A2)


  # ----- Affichage -----
  if not affichage:
    # Affichage trilateration 2D -----
    fig = plt.figure()
    ax = fig.add_subplot(111, title="Localisation de A3")
    ax.set_aspect('equal', adjustable='box')
    ax.set_xlabel('X_{trig}')
    ax.set_ylabel('Y_{trig}')
    plt.grid(True)

    # Points
    ax.scatter(*O_trig, color='r', label='O')
    ax.scatter(*A1_trig, color='g', label='A1')
    ax.scatter(*T_trig, color='b', label='T')

    # Cercles
    circle1 = plt.Circle(O_trig, O_A1, color='r', fill=False, label='Cercle 1')
    circle2 = plt.Circle(A1_trig, A1_A3, color='g', fill=False, label='Cercle 2')
    circle3 = plt.Circle(T_trig, A1_A3, color='b', fill=False, label='Cercle 3')

    ax.add_artist(circle1)
    ax.add_artist(circle2)
    ax.add_artist(circle3)
    
    ax.legend()
    plt.show()


    # Affichage trilateration 3D -----
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d', title="Localisation de A2 & A4")
    ax.set_aspect('equal', adjustable='box')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')

    # Points
    ax.scatter(*O_global, color='black', label='O')
    ax.scatter(*A1_global, color='blue', label='A1')
    ax.scatter(*A3_global, color='green', label='A3')

    # Sphères
    plot_sphere(ax, O_global, O_A1, color='black', alpha=0.3)
    plot_sphere(ax, A1_global, A1_A3, color='blue', alpha=0.3)
    plot_sphere(ax, T_global, A1_A3, color='green', alpha=0.3)

    # Cercles d'intersection
    plot_circle_intersection(O_global, O_A1, A1_global, A1_A3, ax)
    plot_circle_intersection(A1_global, A1_A3, T_global, A1_A3, ax)

    # Pointers d'intersection
    ax.scatter(*A2_global, s=100, facecolors='none', edgecolors='k', linewidth=2, label=f'Intersection: A2')
    ax.scatter(*A4_global, s=100, facecolors='none', edgecolors='k', linewidth=2, label=f'Intersection: A4')


    # Affichage du bras 1 entièrement -----
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d', title="Point du bras 1 dans le repère global")
    ax.set_aspect('equal', adjustable='box')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')

    # Points
    ax.scatter(*O_global, color='black', label='O')
    ax.scatter(*A1_global, color='blue', label='A1')
    ax.scatter(*A2_global, color='red', label='A2')
    ax.scatter(*A3_global, color='green', label='A3')
    ax.scatter(*A4_global, color='red', label='A4')
    ax.scatter(*T_global, color='black', label='O')

    # Segments 
    ax.plot(*np.array([A1_global, A2_global]).T, color='back')
    ax.plot(*np.array([A1_global, A4_global]).T, color='back')
    ax.plot(*np.array([A2_global, A3_global]).T, color='back')
    ax.plot(*np.array([A4_global, A3_global]).T, color='back')


  return A2_global, A3_global, A4_global



def bras_2(T_global):
  # Coordonnées du point B1 (origine du bras 2) positionné suivant y
  O_global = np.array([0, 0, 0])
  B1_global = np.array([0, O_B1 * cos(theta20), O_B1 * sin(theta20)])


  # ----- Point B3 -----
  B1_B3 = sqrt(B1_B2**2 + B1_B4**2 - 2 * B1_B2 * B1_B4 * cos(beta[1]))

  # Repère trigonométrique
  x_trig = B1_global / np.linalg.norm(B1_global)
  z_trig = np.cross(T_global.flatten(), y_trig.flatten()) / np.linalg.norm(np.cross(T_global.flatten(), y_trig.flatten()))
  y_trig = np.cross(y_trig, z_trig)

  O_trig = np.array([0, 0])
  B1_trig = np.array([B1_global.T @ x_trig, B1_global.T @ y_trig])
  T_trig = np.array([T_global.T @ x_trig, T_global.T @ y_trig])

  # Trilateration
  B3_trig = trilateration_2D(O_trig, O_B1, B1_trig, B1_B3, T_trig, B1_B3)
  B3_global = O_global + B3_trig[0] * x_trig + B3_trig[1] * y_trig


  # ----- Point B2 & B4 -----
  B2_global, B4_global = trilateration_3D(O_global, O_B1, B1_global, B1_B2, B3_global, B1_B2)


  # ----- Affichage -----
  # Affichage trilateration 2D -----

  # Affichage trilateration 3D -----
  fig = plt.figure()
  ax = fig.add_subplot(111, projection='3d', title="Point du bras 2")
  ax.set_aspect('equal', adjustable='box')
  ax.set_xlabel('X')
  ax.set_ylabel('Y')
  ax.set_zlabel('Z')

  # Points
  ax.scatter(*O_global, color='black', label='O')
  ax.scatter(*B1_global, color='blue', label='B1')
  ax.scatter(*B2_global, color='red', label='B2')
  ax.scatter(*B3_global, color='green', label='B3')
  ax.scatter(*B4_global, color='red', label='B4')
  ax.scatter(*T_global, color='black', label='O')

  # Segments 
  ax.plot(*np.array([B1_global, B2_global]).T, color='black')
  ax.plot(*np.array([B1_global, B4_global]).T, color='black')
  ax.plot(*np.array([B2_global, B3_global]).T, color='black')
  ax.plot(*np.array([B4_global, B3_global]).T, color='black')

  return B2_global, B3_global, B4_global



# ----------------- Main -----------------
if __name__ == '__main__':
  # Position de l'organe terminal
  T_global = np.array([0, 0, O_T])

  # Calcul des coordonnées des points du bras 1
  A2_global, A3_global, A4_global = bras_1(T_global)

  # Calcul des coordonnées des points du bras 2
  B2_global, B3_global, B4_global = bras_2(T_global)

  plt.show()