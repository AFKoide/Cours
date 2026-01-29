#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

from std_msgs.msg import Float64MultiArray
from sensor_msgs.msg import JointState

import math
import numpy as np
import matplotlib.pyplot as plt  # MATLAB plotting functions
import sympy as sp



# Constants
sin = math.sin
cos = math.cos
pi = math.pi
duree = 40.0
a1 = 0.0771 ; a2 = 0.05 ; a3 = 0.004 ; a4 = 0.0284 ; a5 = 0.03625 ; a6 = 0.0185 ; a7 = 0.11175 ; a8 = -0.01 ; a9 = 0.15

l_arm = ['l_shoulder_y', 'l_shoulder_x', 'l_arm_z', 'l_elbow_y']

class PoppyController(Node):
    def __init__(self):
        super().__init__('poppy_controller')

        self.get_logger().info("Starting the controller")
        self.declare_parameter("type", "robot type, either humanoid (full robot) or arms (left and right arms only)")
        self.robot_type_ = str(self.get_parameter("type").value)
        self.cmd_publisher_ = self.create_publisher(Float64MultiArray, '/joint_group_position_controller/commands', 10)
        self.joint_state_subscription_ = self.create_subscription(JointState, '/joint_states', self.joint_state_callback, 10)

        joint_count = 25 if self.robot_type_ == "humanoid" else 8
        self.cmd_ = Float64MultiArray()
        for _ in range(joint_count):
            self.cmd_.data.append(0)

        self.wait_for_initial_position = True



    # Called after the a JointState message arrives if self.wait_for_initial_position is True
    def init(self):
        # set a non-singular initial pose
        self.set_motor_position('l_shoulder_y', 0.0*pi/180.0)           # Attention : l_shoulder_y est supposé être le premier moteur or pour la commande set_motor_position, la commande attaque le moteur 3
        self.set_motor_position('l_shoulder_x', 0.0*pi/180.0)           # l-shoulder_x attaque le moteur 4
        self.set_motor_position('l_arm_z',      90.0*pi/180.0)               # l_arm_z attaque le moteur 2
        self.set_motor_position('l_elbow_y',    90.0*pi/180.0)             # l_elbow_y attaque le moteur  1 
        # attention la commande get_moteur_position est bien parameté, càd que get_motor_position('l_shoulder_y') récupère la valeur du premier moteur

        self.cmd_publisher_.publish(self.cmd_)

        # Wait until the initial pose is reached
        while True:
            error = 0.0
            #for i in range(len(self.cmd_.data)):
            #    error += math.fabs(self.cmd_.data[i] -self.joint_positions_[i])
            # le calcul de l'erreur a été modifié pour se conformer aux remarques précédentes --> si la commande set_motor_position est réglé, on peut revenir à la définition de l'erreur ci-dessus
            error = math.fabs(self.cmd_.data[0] - self.joint_positions_[2]) + math.fabs(self.cmd_.data[1] - self.joint_positions_[3]) + math.fabs(self.cmd_.data[2] - self.joint_positions_[1]) + math.fabs(self.cmd_.data[3] - self.joint_positions_[0])
            if error < 0.01:
                break
            else:
                return

        self.wait_for_initial_position = False
        self.t0 = self.get_time()
        self.get_logger().info("Reached initial joint position (%s), starting the control loop" % self.joint_positions_)

        self.X_init = self.MGD(self.get_motor_position(l_arm[0]), self.get_motor_position(l_arm[1]), self.get_motor_position(l_arm[2]), self.get_motor_position(l_arm[3]))


        self.q_p =  np.array([[90.0*pi/180.0],
                              [90.0*pi/180.0],
                              [0.0*pi/180.0],
                              [0.0*pi/180.0]])

        self.x_trajectoire = None


        # Init done, now start the control loop
        self.run_timer = self.create_timer(0.1, self.run)



# Pour les PLOTS:
        self.plot_temps   = np.array([])

        self.plot_desiree = np.array([[],
                                      [],
                                      []])
        self.plot_actuel  = self.plot_desiree
        self.plot_vitesse = self.plot_desiree

        self.plot_angle   = np.array([[],
                                      [],
                                      [],
                                      []])
        self.plot_q       = self.plot_angle
        self.plot_qd      = self.plot_angle





    def run(self):
        dt = self.get_time() - self.t0
        self.get_logger().info("t = %s" %dt)
        self.plot_temps = np.hstack((self.plot_temps, dt)) # Pour le Plot

        # Position désirée du bras
        Xd = [0.2615, 0.1740, 0.0540]

        self.x_trajectoire = self.Generation_Trajectoire(Xd, self.X_init, dt)   # X désiré
        # self.get_logger().info("Trajectoire = %s" % self.x_trajectoire.reshape(3,1))
        

        if dt < duree:
            # TODO : Ecrire ici votre boucle de commande
#            X = self.MGD(theta1, theta2, theta3, theta4)
#            Jacobienne = self.Jacob(theta1, theta2, theta3, theta4)

            self.X = self.Commande_Cine()
#            self.get_logger().info("X: %s" % self.X)

            self.cmd_publisher_.publish(self.cmd_)
#            self.get_logger().info("cmd = %s" %self.cmd_)
        else:
            self.plot()



    # Génération de la trajectoire
    def Generation_Trajectoire(self, Xd, Xinit, dt):
        tf = 40
        trajectoire = Xinit + (Xd - Xinit) * (10*pow((dt/tf), 3) - 15*pow((dt/tf), 4) + 6*pow((dt/tf), 5))

        return trajectoire


    # Q1: Modèle Géométrique Direct
    def MGD(self, theta1, theta2, theta3, theta4):
        x = a1 + a4 + a5*cos(theta2) + a7*cos(theta2) + a8*(sin(theta2) * sin(theta3)) + a9*(cos(theta2)*cos(theta4) + sin(theta2)*sin(theta3)*sin(theta4))
        y = (-sin(theta1)*sin(theta2)*cos(theta4) + (sin(theta1)*cos(theta2)*sin(theta3) - cos(theta1)*cos(theta3)) * sin(theta4))*a9 - sin(theta1)*sin(theta2)*a7 + (sin(theta1)*cos(theta2)*sin(theta3) - cos(theta1)*cos(theta3))*a8 - sin(theta1)*sin(theta2)*a5 - cos(theta1)*a6 + a2
        z = (cos(theta1)*sin(theta2)*cos(theta4) + (-cos(theta1)*cos(theta2)*sin(theta3) - sin(theta1)*cos(theta3))*sin(theta4))*a9 + cos(theta1)*sin(theta2)*a7 + (-cos(theta1)*cos(theta2)*sin(theta3) - sin(theta1)*cos(theta3))*a8 + cos(theta1)*sin(theta2)*a5 - sin(theta1)*a6 + a3

        # TODO: return X: vecteur contenant la position de l'organe terminal
        X = np.array([x, y, z])

        return X


    # Q2: Jacobien 
    def Jacob(self, theta1_val, theta2_val, theta3_val, theta4_val):
        theta1, theta2, theta3, theta4 = sp.symbols('theta1 theta2 theta3 theta4')  # Déclare les angles en symbolique

        x = a1 + a4 + a5*sp.cos(theta2) + a7*sp.cos(theta2) + a8*(sp.sin(theta2) * sp.sin(theta3)) + a9*(sp.cos(theta2)*sp.cos(theta4) + sp.sin(theta2)*sp.sin(theta3)*sp.sin(theta4))
        y = (-sp.sin(theta1)*sp.sin(theta2)*sp.cos(theta4) + (sp.sin(theta1)*sp.cos(theta2)*sp.sin(theta3) - sp.cos(theta1)*sp.cos(theta3)) * sp.sin(theta4))*a9 - sp.sin(theta1)*sp.sin(theta2)*a7 + (sp.sin(theta1)*sp.cos(theta2)*sp.sin(theta3) - sp.cos(theta1)*sp.cos(theta3))*a8 - sp.sin(theta1)*sp.sin(theta2)*a5 - sp.cos(theta1)*a6 + a2
        z = (sp.cos(theta1)*sp.sin(theta2)*sp.cos(theta4) + (-sp.cos(theta1)*sp.cos(theta2)*sp.sin(theta3) - sp.sin(theta1)*sp.cos(theta3))*sp.sin(theta4))*a9 + sp.cos(theta1)*sp.sin(theta2)*a7 + (-sp.cos(theta1)*sp.cos(theta2)*sp.sin(theta3) - sp.sin(theta1)*sp.cos(theta3))*a8 + sp.cos(theta1)*sp.sin(theta2)*a5 - sp.sin(theta1)*a6 + a3


        fonction = [x, y, z]
        thetas = [theta1, theta2, theta3, theta4]
        J_num = np.zeros([3,4])
        for i in range(3):  # parcourt les lignes de la matrice
            for j in range(4):  # parcourt les colonnes de la matrice
                J = sp.diff(fonction[i], thetas[j]) # Dérivation
                
                temp = J.subs({theta1: theta1_val, theta2: theta2_val, theta3: theta3_val, theta4: theta4_val}) # Substitue les theta symboliques avec les valeurs réelles
                J_num[i, j] = temp.evalf()      # Remplie la matrice J
        
        return J_num


# Q3: Modèle Cinématique Direct
    def Commande_Cine(self):
    # Récupère l'angle actuel
        theta1 = self.get_motor_position(l_arm[0]) ; theta2 = self.get_motor_position(l_arm[1]) ; theta3 = self.get_motor_position(l_arm[2]) ; theta4 = self.get_motor_position(l_arm[3]) ; 
        self.plot_angle = np.hstack((self.plot_angle , np.array([[theta1],
                                                                 [theta2],
                                                                 [theta3],
                                                                 [theta4]])))


    # Recupere la trajectoire 
        Xd = self.x_trajectoire.reshape(3,1)
        # self.get_logger().info("Xd: %s" % Xd)
        self.plot_desiree = np.hstack((self.plot_desiree, Xd))

    # Recupere la valeur actuelle
        X = self.MGD(theta1, theta2, theta3, theta4).reshape(3,1)
        # self.get_logger().info("X: %s" % X)
        self.plot_actuel = np.hstack((self.plot_actuel, X))

    # Calcul la jacobienne
        Jacobienne = self.Jacob(theta1, theta2, theta3, theta4)
        # self.get_logger().info("Jacobienne: %s" % Jacobienne)

    # TODO: TROUVER LE MEILLEUR KP
        Kp = np.identity(3) * 0.39


        xd_point = Kp @ (Xd - X)
#        self.get_logger().info("xd_point: %s" % xd_point)
        self.plot_vitesse = np.hstack((self.plot_vitesse, xd_point.reshape(3,1)))

        q_point = np.linalg.pinv(Jacobienne) @ xd_point.reshape(3,1) # Calcul de la pseudo inverse
#        self.get_logger().info("q_point: %s" % q_point)
        self.plot_qd = np.hstack((self.plot_qd, q_point.reshape(4,1)))

        q = q_point*0.1 + self.q_p
#        self.get_logger().info("q: %s" % q)
        self.plot_q = np.hstack((self.plot_q, q.reshape(4,1)))
        self.q_p = q


    # Servomoteur
        self.set_motor_position('l_shoulder_y', q[2])               # Attention : l_shoulder_y est supposé être le premier moteur or pour la commande set_motor_position, la commande attaque le moteur 3
        self.set_motor_position('l_shoulder_x', q[3])               # l-shoulder_x attaque le moteur 4
        self.set_motor_position('l_arm_z',      q[1])               # l_arm_z attaque le moteur 2
        self.set_motor_position('l_elbow_y',    q[0])               # l_elbow_y attaque le moteur  1 

        return X


    def plot(self):
        # self.get_logger().info("Taille de t = %s" % len(self.plot_temps[0 : len(self.plot_actuel[0])]))
        # self.get_logger().info("Taille de x = %s" % len(self.plot_desiree[0][0 : len(self.plot_actuel[0])]))
        # self.get_logger().info("Taille de x = %s" % len(self.plot_actuel[0]))
        
        plt.figure("Plot des positions cartésiennes")
# Plot la position désirée et la position actuel
        plt.subplot(2,1,1)
        plt.title("Position désirée et position actuel en fonction du temps")
        plt.plot(self.plot_temps[0 : len(self.plot_actuel[0])], self.plot_desiree[0][0 : len(self.plot_actuel[0])], "r", label="x désiré")  ;  plt.plot(self.plot_temps[0 : len(self.plot_actuel[0])], self.plot_actuel[0][:], "r--", label="x actuel")
        plt.plot(self.plot_temps[0 : len(self.plot_actuel[0])], self.plot_desiree[1][0 : len(self.plot_actuel[0])], "g", label="y désiré")  ;  plt.plot(self.plot_temps[0 : len(self.plot_actuel[0])], self.plot_actuel[1][:], "g--", label="y actuel")
        plt.plot(self.plot_temps[0 : len(self.plot_actuel[0])], self.plot_desiree[2][0 : len(self.plot_actuel[0])], "b", label="z désiré")  ;  plt.plot(self.plot_temps[0 : len(self.plot_actuel[0])], self.plot_actuel[2][:], "b--", label="z actuel")
        plt.grid("minor")
        plt.legend()
        plt.xlabel("Temps (s)")

# Plot la vitesse articulaire
        plt.subplot(2,1,2)
        plt.title("Variation de x_{point} en fonction du temps")
        plt.plot(self.plot_temps[0 : len(self.plot_vitesse[0])], self.plot_vitesse[0][0 : len(self.plot_vitesse[0])], "r", label="Vitesse de x")
        plt.plot(self.plot_temps[0 : len(self.plot_vitesse[0])], self.plot_vitesse[1][0 : len(self.plot_vitesse[0])], "g", label="Vitesse de y")
        plt.plot(self.plot_temps[0 : len(self.plot_vitesse[0])], self.plot_vitesse[2][0 : len(self.plot_vitesse[0])], "b", label="Vitesse de z")
        plt.grid("minor") ; plt.xlabel("Temps (s)") ; plt.ylabel("Vitesse")
        plt.legend()


        plt.figure("Plot des Angles")
# Plot les angles
        plt.subplot(2,1,1)
        plt.title("Angle désirée et actuel en fonction du temps")
        plt.plot(self.plot_temps[0 : len(self.plot_angle[0])], self.plot_angle[0][0 : len(self.plot_angle[0])], "r", label="Angle actuel de l_shoulder_y")  ;  plt.plot(self.plot_temps[0 : len(self.plot_q[0])], self.plot_q[0][0 : len(self.plot_q[0])], "r--", label="Angle désiré de l_shoulder_y")
        plt.plot(self.plot_temps[0 : len(self.plot_angle[0])], self.plot_angle[1][0 : len(self.plot_angle[0])], "g", label="Angle actuel de l_shoulder_x")  ;  plt.plot(self.plot_temps[0 : len(self.plot_q[0])], self.plot_q[1][0 : len(self.plot_q[0])], "g--", label="Angle désiré de l_shoulder_x")
        plt.plot(self.plot_temps[0 : len(self.plot_angle[0])], self.plot_angle[2][0 : len(self.plot_angle[0])], "b", label="Angle actuel de l_arm_z")       ;  plt.plot(self.plot_temps[0 : len(self.plot_q[0])], self.plot_q[2][0 : len(self.plot_q[0])], "b--", label="Angle désiré de l_arm_z")
        plt.plot(self.plot_temps[0 : len(self.plot_angle[0])], self.plot_angle[3][0 : len(self.plot_angle[0])], "k", label="Angle actuel de l_elbow_y")     ;  plt.plot(self.plot_temps[0 : len(self.plot_q[0])], self.plot_q[3][0 : len(self.plot_q[0])], "k--", label="Angle désiré de l_elbow_y")
        plt.grid("minor") ; plt.legend()
        plt.xlabel("Temps (s)") ; plt.ylabel("Angle (radian)")

# Plot q point
        plt.subplot(2,1,2)
        plt.title("Vitesse angulaire en fonction du temps")
        plt.plot(self.plot_temps[0 : len(self.plot_qd[0])], self.plot_qd[0][0 : len(self.plot_qd[0])], "r", label="Vitesse q de l_shoulder_y")
        plt.plot(self.plot_temps[0 : len(self.plot_qd[0])], self.plot_qd[1][0 : len(self.plot_qd[0])], "g", label="Vitesse q de l_shoulder_x")
        plt.plot(self.plot_temps[0 : len(self.plot_qd[0])], self.plot_qd[2][0 : len(self.plot_qd[0])], "b", label="Vitesse q de l_arm_z")
        plt.plot(self.plot_temps[0 : len(self.plot_qd[0])], self.plot_qd[3][0 : len(self.plot_qd[0])], "k", label="Vitesse q de l_elbow_y")
        plt.grid("minor") ; plt.legend()
        plt.xlabel("Temps (s)") ; plt.ylabel("Vitesse angulaire (radian/s)")


        plt.show()

        self.destroy_node()









    def get_motor_position(self, joint_name):
        index = self.joint_index(joint_name)
        return self.joint_positions_[index]



    def set_motor_position(self, joint_name, joint_pos):
        index = self.joint_index(joint_name)
        self.cmd_.data[index] = joint_pos



    def joint_state_callback(self, msg):
        self.joint_names_ = msg.name
        self.joint_positions_ = msg.position
        if self.wait_for_initial_position:
            self.init()



    def joint_index(self, joint_name):
        return self.joint_names_.index(joint_name)



    def get_time(self):
        sec_nsec = self.get_clock().now().seconds_nanoseconds()
        return sec_nsec[0] + 1e-9 * sec_nsec[1]






def main(args=None):
    rclpy.init(args=args)

    poppy_controller = PoppyController()

    try:
        rclpy.spin(poppy_controller)
    except KeyboardInterrupt:
        pass
    finally:
        rclpy.shutdown()






if __name__ == '__main__':
    main()

#  poppy control pour commande ré