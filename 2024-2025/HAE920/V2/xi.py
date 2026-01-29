import numpy as np
import matplotlib.pyplot as plt

# Define the trajectory generation function
def GenerationTrajectoire(t, tf, l, X_i, R_i, R, theta_angle, X_f):
    # Fifth-degree polynomial for smooth trajectory
    r = 10 * (t / tf) ** 3 - 15 * (t / tf) ** 4 + 6 * (t / tf) ** 5
    r_dot = 30 * t ** 2 / tf ** 3 - 60 * t ** 3 / tf ** 4 + 30 * t ** 4 / tf ** 5

    # Desired position and velocity
    x_desire = X_i + r * (X_f - X_i)
    v_desire = (X_f - X_i) * r_dot

    # Rotation interpolation
    rot_matrix = np.array([
        [pow(R[0] / l, 2) * (1 - np.cos(r * theta_angle)) + np.cos(r * theta_angle),
         R[0] / l * R[1] / l * (1 - np.cos(r * theta_angle)) - R[2] / l * np.sin(r * theta_angle),
         R[0] / l * R[2] / l * (1 - np.cos(r * theta_angle)) + R[1] / l * np.sin(r * theta_angle)],
        [R[0] / l * R[1] / l * (1 - np.cos(r * theta_angle)) + R[2] / l * np.sin(r * theta_angle),
         pow(R[1] / l, 2) * (1 - np.cos(r * theta_angle)) + np.cos(r * theta_angle),
         R[1] / l * R[2] / l * (1 - np.cos(r * theta_angle)) - R[0] / l * np.sin(r * theta_angle)],
        [R[0] / l * R[2] / l * (1 - np.cos(r * theta_angle)) - R[1] / l * np.sin(r * theta_angle),
         R[1] / l * R[2] / l * (1 - np.cos(r * theta_angle)) + R[0] / l * np.sin(r * theta_angle),
         pow(R[2] / l, 2) * (1 - np.cos(r * theta_angle)) + np.cos(r * theta_angle)]
    ])
    r_desire = R_i @ rot_matrix

    return x_desire, r_desire, v_desire, r_dot

# Testing parameters
tf = 5  # Final time
l = 1.0  # Norm of the rotation axis (assumed unitary)
X_i = np.array([0, 0, 0])  # Initial position
X_f = np.array([1, 1, 1])  # Final position
R_i = np.eye(3)  # Initial rotation (identity matrix)
R = np.array([0, 0, 1])  # Axis of rotation
theta_angle = np.pi / 2  # Rotation angle

# Generate trajectory over time
time_steps = np.linspace(0, tf, 100)
positions = []
velocities = []
rotations = []

for t in time_steps:
    x_desire, r_desire, v_desire, r_dot = GenerationTrajectoire(t, tf, l, X_i, R_i, R, theta_angle, X_f)
    positions.append(x_desire)
    velocities.append(v_desire)
    rotations.append(r_desire)

positions = np.array(positions)
velocities = np.array(velocities)
rotations = np.array(rotations)

# Plotting results
plt.figure(figsize=(10, 6))
plt.plot(time_steps, positions[:, 0], label="X")
plt.plot(time_steps, positions[:, 1], label="Y")
plt.plot(time_steps, positions[:, 2], label="Z")
plt.title("Desired Position Over Time")
plt.xlabel("Time [s]")
plt.ylabel("Position [m]")
plt.legend()
plt.grid()
plt.show()

plt.figure(figsize=(10, 6))
plt.plot(time_steps, velocities[:, 0], label="Vx")
plt.plot(time_steps, velocities[:, 1], label="Vy")
plt.plot(time_steps, velocities[:, 2], label="Vz")
plt.title("Desired Velocity Over Time")
plt.xlabel("Time [s]")
plt.ylabel("Velocity [m/s]")
plt.legend()
plt.grid()
plt.show()

# Display rotation matrices at key moments
print("Initial Rotation Matrix:\n", rotations[0])
print("\nMiddle Rotation Matrix:\n", rotations[len(rotations) // 2])
print("\nFinal Rotation Matrix:\n", rotations[-1])
