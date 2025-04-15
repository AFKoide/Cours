import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import minimize

# Définition de la trilatération avec optimisation
def trilateration_optimization(spheres):
    def error_function(point):
        x, y, z = point
        return sum((np.sqrt((x - cx)**2 + (y - cy)**2 + (z - cz)**2) - r)**2 for cx, cy, cz, r in spheres)
    
    # Point initial (moyenne des centres)
    x0 = np.mean([cx for cx, _, _, _ in spheres])
    y0 = np.mean([cy for _, cy, _, _ in spheres])
    z0 = np.mean([cz for _, _, cz, _ in spheres])
    initial_guess = [x0, y0, z0]
    
    # Stocker l'évolution de l'optimisation
    history = []
    
    def callback(xk):
        history.append(xk)
    
    # Optimisation
    result = minimize(error_function, initial_guess, method='Nelder-Mead', callback=callback)
    
    return result.x if result.success else None, history

# Définition des sphères
spheres = [
    (0, 0, 0, 5),
    (5, 0, 0, 5),
    (0, 5, 0, 5)
]

# Exécution de l'optimisation
point_estime, history = trilateration_optimization(spheres)

# Conversion de l'historique en numpy array pour le tracé
history = np.array(history)

# Création du plot
fig = plt.figure(figsize=(8, 6))
ax = fig.add_subplot(111, projection='3d')

# Tracer les sphères (leurs centres)
for cx, cy, cz, r in spheres:
    ax.scatter(cx, cy, cz, color='red', s=100, label="Centre de sphère" if cx == 0 else "")

# Tracer le chemin de l'optimisation
ax.plot(history[:, 0], history[:, 1], history[:, 2], marker='o', linestyle='-', color='blue', label="Optimisation")

# Tracer le point final trouvé
ax.scatter(*point_estime, color='green', s=100, label="Point trouvé")

# Réglages du plot
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title("Processus d'optimisation pour la trilatération")
ax.legend()

# Afficher le plot
plt.show()
