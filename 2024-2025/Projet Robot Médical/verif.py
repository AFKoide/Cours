import numpy as np
import matplotlib.pyplot as plt
from numpy import cos, sin

# Définition des constantes
theta10 = np.radians(20)  # Angle du bras 1
theta20 = np.radians(20)
O_A1 = 122.5
O_B1 = 142.5

# Coordonnée du point A1 dans le repère global
A1_global = np.array([O_A1 * cos(theta10), 0, O_A1 * sin(theta10)])

# Calcul du repère trigonométrique de A1
x_trig_A1 = A1_global / np.linalg.norm(A1_global)
z_trig_A1 = np.cross(np.array([0, 0, 1]), x_trig_A1)
z_trig_A1 /= np.linalg.norm(z_trig_A1)
y_trig_A1 = np.cross(z_trig_A1, x_trig_A1)

# Conversion de A1 au repère trig
A1_trig_x = np.dot(A1_global, x_trig_A1)
A1_trig_y = np.dot(A1_global, y_trig_A1)
A1_trig = np.array([A1_trig_x, A1_trig_y])

# Recalcul de A1 dans le repère global
A1_recalculated = A1_trig_x * x_trig_A1 + A1_trig_y * y_trig_A1

# ----------------------------------------------------------------
B1_global = np.array([0, O_B1 * cos(theta20), O_B1 * sin(theta20)])

# Calcul du repère trigonométrique de B1
x_trig_B1 = B1_global / np.linalg.norm(B1_global)
z_trig_B1 = np.cross(np.array([0, 1, 0]), x_trig_B1)
y_trig_B1 = np.cross(z_trig_B1, x_trig_B1)

# Conversion de B1 au repère trig
B1_trig_x = np.dot(B1_global, x_trig_B1)
B1_trig_y = np.dot(B1_global, y_trig_B1)
B1_trig = np.array([B1_trig_x, B1_trig_y])

# Recalcul de B1 dans le repère global
B1_recalculated = B1_trig_x * x_trig_B1 + B1_trig_y * y_trig_B1

# Affichage
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d', title="Repère trig et A1")
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

# Points
ax.scatter(*A1_global, color='blue', label='A1')
ax.scatter(*B1_global, color='blue', label='B1')
ax.scatter(*A1_recalculated, color='cyan', label='A1 recalculated')
ax.scatter(*B1_recalculated, color='magenta', label='B1 recalculated')
ax.scatter(0, 0, 0, color='black', label='Origine')

# Segment A1 - Origine
ax.plot([0, A1_global[0]], [0, A1_global[1]], [0, A1_global[2]], color='red', label='A1_0')
ax.plot([0, B1_global[0]], [0, B1_global[1]], [0, B1_global[2]], color='red', label='B1_0')

# Segment uniquement en X
ax.plot([0, A1_global[0]], [0, 0], [0, 0], color='green', label='X_vector')

ax.legend()
plt.show()
