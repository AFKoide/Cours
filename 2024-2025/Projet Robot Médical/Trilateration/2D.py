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




# --------------------------------------------------------------------------------------------------------------------------------
def Trilateration_2D(center1, radius1, center2, radius2, center3, radius3, tol=1e-2):
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
  
  # return None  # Pas d'intersection trouvée



# Exemple d'utilisation
beta[0] = np.radians(79.2)
theta1 = np.radians(90 - 20)

center1 = np.array([0, 0])      # Origine du repère
center2 = np.array([O_A1, 0])   # A1
center3 = np.array([O_T * cos(theta1), O_T * sin(theta1)])    # T

A1_A3 =   np.linalg.norm(center3 - center2) / 2

radius1 = O_A1
radius2 = A1_A3
radius3 = A1_A3

point = Trilateration_2D(center1, radius1, center2, radius2, center3, radius3)

# Affichage des cercles et du point d'intersection
fig, ax = plt.subplots()
circle1 = plt.Circle(center1, radius1, color='r', fill=False)
circle2 = plt.Circle(center2, radius2, color='g', fill=False)
circle3 = plt.Circle(center3, radius3, color='b', fill=False)

ax.add_artist(circle1)
ax.add_artist(circle2)
ax.add_artist(circle3)

# ax.plot(point[0], point[1], 'ko', label='A3: {:.2f}, {:.2f}'.format(*point))

ax.scatter(*center1, color='r')
ax.text(*center1, 'O', ha='right')
ax.scatter(*center2, color='g')
ax.text(*center2, 'A1', ha='right')
ax.scatter(*center3, color='b')
ax.text(*center3, 'T', ha='right')

# Affiche le repère
ax.quiver(0, 0, 50, 0, color='k', scale=2, scale_units='xy', angles='xy')
ax.quiver(0, 0, 0, 50, color='k', scale=2, scale_units='xy', angles='xy')


ax.set_xlim([-150, 300])
ax.set_ylim([-150, 300])

ax.set_aspect('equal', adjustable='box')
ax.legend()
plt.xlabel('X_trig')
plt.ylabel('Y_trig')
plt.title('Trilateration 2D')
plt.grid(True)
plt.show()

# ----------------------------------------
# Fonction pour trouver la position de A3
"""
def find_A3(T_global):
  # Calcul de la coordonnée de A1 dans le repère global
  A1_global = np.array([O_A1 * cos(theta10), 0, O_A1 * sin(theta10)])

  # Repère trig 
  x_trig = A1_global / np.linalg.norm(A1_global)
  T_cross_x = np.cross(T_global.flatten(), x_trig.flatten())
  z_trig = T_cross_x / np.linalg.norm(T_cross_x)
  y_trig = np.cross(z_trig, x_trig)

  Vx = T_global.T @ x_trig
  Vy = T_global.T @ y_trig

  O_trig = np.array([0, 0])
  A1_trig = np.array([O_A1, 0])
  T_trig = np.array([Vx, Vy])

  # Trilateration 2D
  A1_A3 = sqrt(A1_A2**2 + A1_A2**2 - 2*(A1_A2**2)*cos(beta[0]))

  A3 = Trilateration_2D(O_trig, O_A1, A1_trig, A1_A3, T_trig, A1_A3)
  
  return A3
"""