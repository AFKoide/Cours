import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def plot_sphere(ax, center, radius, color='b', alpha=0.3):
    u, v = np.mgrid[0:2*np.pi:100j, 0:np.pi:50j]
    x = center[0] + radius * np.cos(u) * np.sin(v)
    y = center[1] + radius * np.sin(u) * np.sin(v)
    z = center[2] + radius * np.cos(v)
    ax.plot_surface(x, y, z, color=color, alpha=alpha)

def find_intersection_point(center1, radius1, center2, radius2, center3, radius3):
    A = np.array(center1)
    B = np.array(center2)
    C = np.array(center3)
    
    d12 = np.linalg.norm(B - A)
    d13 = np.linalg.norm(C - A)
    d23 = np.linalg.norm(C - B)
    
    ex = (B - A) / d12
    i = np.dot(ex, (C - A))
    ey = (C - A - i * ex) / np.linalg.norm(C - A - i * ex)
    ez = np.cross(ex, ey)
    
    x = (radius1**2 - radius2**2 + d12**2) / (2 * d12)
    y = (radius1**2 - radius3**2 + d13**2 - 2 * i * x) / (2 * np.dot(ey, C - A))
    z_squared = radius1**2 - x**2 - y**2
    
    if z_squared < 0:
        print("No real intersection point exists.")
        return None
    
    z = np.sqrt(z_squared)
    
    intersection1 = A + x * ex + y * ey + z * ez
    intersection2 = A + x * ex + y * ey - z * ez
    
    return intersection1, intersection2

def plot_circle_intersection(centerA, radiusA, centerB, radiusB, ax):
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

def plot_intersection_spheres(center1, radius1, center2, radius2, center3, radius3):
    fig = plt.figure(figsize=(10, 7))
    ax = fig.add_subplot(111, projection='3d')
    
    # Plot the three spheres
    plot_sphere(ax, center1, radius1, color='r')
    plot_sphere(ax, center2, radius2, color='g')
    plot_sphere(ax, center3, radius3, color='b')
    
    # Plot intersections of each pair of spheres
    plot_circle_intersection(center1, radius1, center2, radius2, ax)
    plot_circle_intersection(center1, radius1, center3, radius3, ax)
    plot_circle_intersection(center2, radius2, center3, radius3, ax)
    
    # Compute and plot the final intersection points
    intersection_points = find_intersection_point(center1, radius1, center2, radius2, center3, radius3)
    
    if intersection_points:
        for point in intersection_points:
            # ax.scatter(*point, color='w', s=100, edgecolors='k', label=f'Intersection: {point}')
            ax.scatter(*point, s=100, facecolors='none', edgecolors='k', linewidth=2, label=f'Intersection: {point}')

            print(f"Intersection at: {point}")
    
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    ax.set_title("Intersections of Three Spheres")
    plt.legend()
    plt.show()

# Example usage
center1 = (0, 0, 0)
radius1 = 5
center2 = (3, 0, 0)
radius2 = 4
center3 = (1, 4, 0)
radius3 = 4

plot_intersection_spheres(center1, radius1, center2, radius2, center3, radius3)
