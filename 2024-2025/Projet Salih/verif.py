import numpy as np
import matplotlib.pyplot as plt
from numpy import cos, sin, arccos, degrees

# Définition des constantes
theta10 = np.radians(20)  # Angle du bras 1
O_A1 = 122.5

# Coordonnée du point A1 dans le repère global
A1_global = np.array([O_A1 * cos(theta10), 0, O_A1 * sin(theta10)])

# Calcul du repère trigonométrique
x_trig = A1_global / np.linalg.norm(A1_global)
z_trig = np.cross(np.array([0, 0, 1]), x_trig)
z_trig /= np.linalg.norm(z_trig)
y_trig = np.cross(z_trig, x_trig)

# Conversion de A1 au repère trig
A1_trig_x = np.dot(A1_global, x_trig)
A1_trig_y = np.dot(A1_global, y_trig)
A1_trig = np.array([A1_trig_x, A1_trig_y])

# Vecteur ayant uniquement une composante en x
x_vector = np.array([1, 0, 0])

# Calcul de l'angle entre A1_global et l'axe x pur
dot_product = np.dot(A1_global / np.linalg.norm(A1_global), x_vector)
angle = degrees(arccos(dot_product))

# Affichage
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d', title="Repère trig et A1")
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

# Points
ax.scatter(*A1_global, color='blue', label='A1')
ax.scatter(0, 0, 0, color='black', label='Origine')

# Segment A1 - Origine
ax.plot([0, A1_global[0]], [0, A1_global[1]], [0, A1_global[2]], color='red', label='A1_0')

# Segment uniquement en X
ax.plot([0, A1_global[0]], [0, 0], [0, 0], color='green', label='X_vector')

# Affichage de l'angle
print(f"Angle entre A1_global et l'axe X : {angle:.2f}°")

ax.legend()
plt.show()