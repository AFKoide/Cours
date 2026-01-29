# simRemoteApi.start(19999)

import numpy as np
import sim
import time
import math
import matplotlib.pyplot as plt
from control.matlab import *
import sympy as sp


a2 = 0.612
a3 = 0.5723
r1 = 0.1273
r2 = 0.163941
r5 = 0.1157
r7 = 0.0922
alpha2 = np.pi/2
alpha5 = np.pi/2
alpha6 = -np.pi/2





if __name__ == '__main__':
    print ('Program started')
    sim.simxFinish(-1) # just in case, close all opened connections
    clientID=sim.simxStart('127.0.0.1',19999,True,True,5000,5) # Connect to CoppeliaSim

    h=np.array([0,0,0,0,0,0])
    q = np.array([0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
    

    if clientID!=-1:
        print ('Connected to remote API server')

        # Now try to retrieve data in a blocking fashion (i.e. a service call):
        res,objs=sim.simxGetObjects(clientID,sim.sim_handle_all,sim.simx_opmode_blocking)
        if res==sim.simx_return_ok:
            print ('Number of objects in the scene: ',len(objs))
        else:
            print ('Remote API function call returned with error code: ',res)
            
        # Now retrieve streaming data (i.e. in a non-blocking fashion):
        startTime=time.time()
        sim.simxGetIntegerParameter(clientID,sim.sim_intparam_mouse_x,sim.simx_opmode_streaming) # Initialize streaming
           
        r, h[0]=sim.simxGetObjectHandle(clientID,'UR10_joint1', sim.simx_opmode_blocking)
        r, h[1]=sim.simxGetObjectHandle(clientID,'UR10_joint2', sim.simx_opmode_blocking)
        r, h[2]=sim.simxGetObjectHandle(clientID,'UR10_joint3', sim.simx_opmode_blocking)
        r, h[3]=sim.simxGetObjectHandle(clientID,'UR10_joint4', sim.simx_opmode_blocking)
        r, h[4]=sim.simxGetObjectHandle(clientID,'UR10_joint5', sim.simx_opmode_blocking)
        r, h[5]=sim.simxGetObjectHandle(clientID,'UR10_joint6', sim.simx_opmode_blocking)
        
            
        #############################################################################################################
        #                       Programme de la commande débute ici
        #############################################################################################################
        



        tf = 5 #final time
        dt = 0.05

        # Initialize arrays to store joint positions and end effector data
        joint_positions_all = np.zeros((6, int(tf/dt) + 1))  # To store positions of each joint
        end_effector_positions_all = np.zeros((3, int(tf/dt) + 1))  # To store positions of the end effector
        end_effector_velocities_all = np.zeros((3, int(tf/dt) + 1))  # To store velocities of the end effector


        
        A_f = np.array([np.pi/2,np.pi/4,-np.pi/2,-np.pi/4,np.pi/4,0.])
        
        O67 = np.array([[0],[0],[r7],[1]])


        sim_joint = np.array([17,20,23,26,29,32]) #Coppeprint("J est",J)liaSim joints handles
        modeSpeed = 0
        modePosition = 1
        
        def GetPos(clientID,joints,opmode): #usually opmode = sim.simx_opmode_blocking
            position = np.zeros(len(joints))       
            for i in range(len(joints)):
                _, position[i] = sim.simxGetJointPosition(clientID,joints[i],opmode)
            
            return position
        
        def SetMode(clientID, joints, mode):
            if (mode==0):
                opmode = sim.simx_opmode_oneshot
            else:
                opmode = sim.simx_opmode_streaming
                    
            for i in range(len(joints)):
                sim.simxSetObjectInt32Param(clientID, joints[i],sim.sim_jointintparam_ctrl_enabled,mode,opmode)
                
                # For speed control: immediatly setting speed to 0, otherwise the robot will do unwanted movements while all parameters are being set
                if (mode==0):
                    sim.simxSetJointTargetVelocity(clientID, joints[i], 0, sim.simx_opmode_blocking)
                
        def SetPos(clientID, joints, position):
                if (len(position) == len(joints)):
                    for i in range(len(joints)):
                        sim.simxSetJointTargetPosition(clientID,joints[i],position[i],sim.simx_opmode_blocking)
                        
        def SetSpeed(clientID, joints, velocity):
                if (len(velocity) == len(joints)):
                    for i in range(len(joints)):
                        sim.simxSetJointTargetVelocity(clientID, joints[i], velocity[i], sim.simx_opmode_blocking)

        def MatrixTransformation(uj):
            "print('uj est',uj)"
            rot1 = np.array([[1,0,0,0],[0,np.cos(uj[0]),-np.sin(uj[0]),0],[0,np.sin(uj[0]),np.cos(uj[0]),0],[0,0,0,1]])
            trans1 = np.array([[1,0,0,uj[1]],[0,1,0,0],[0,0,1,0],[0,0,0,1]])
            rot2 = np.array([[np.cos(uj[2]),-np.sin(uj[2]),0,0],[np.sin(uj[2]),np.cos(uj[2]),0,0],[0,0,1,0],[0,0,0,1]])
            trans2 = np.array([[1,0,0,0],[0,1,0,0],[0,0,1,uj[3]],[0,0,0,1]])

            T = rot1@trans1@rot2@trans2
            return T



        K_p = np.zeros((3,3))
        K_o = np.zeros((3,3))

        K_p[0][0] = 2
        K_p[1][1] = 2
        K_p[2][2] = 2

        K_o[0][0] = 1
        K_o[1][1] = 1
        K_o[2][2] = 1


        def Kinematics(joint_pos):
            theta1,theta2,theta3,theta4,theta5,theta6 = joint_pos

            u1 = np.array([0,0,theta1,r1])
            u2 = np.array([alpha2,0,theta2-np.pi/2,r2])
            u3 = np.array([0,-a2,theta3,0])
            u4 = np.array([0,-a3,theta4,0])
            u5 = np.array([alpha5,0,theta5-np.pi/2,r5])
            u6 = np.array([alpha6,0,theta6,0])


            T01 = MatrixTransformation(u1)
            T02 = T01 @ MatrixTransformation(u2)
            T03 = T02 @ MatrixTransformation(u3)
            T04 = T03 @ MatrixTransformation(u4)
            T05 = T04 @ MatrixTransformation(u5)
            T06 = T05 @ MatrixTransformation(u6)
            T07 = T06@O67
            P07 = T07[0:3].reshape(3,1)
            P05 = T05[0:3,3].reshape(3,1)
            T = (T01,T02,T03,T04,T05,T06)
            
            for i in range(0,6):
                T0i = T[i]
                P0i = T0i[0:3,3].reshape(3,1)
                P0i7 = P07 - P0i
                
                Z0i = T0i[0:3,2].reshape(3,1)
                Jup = np.cross(Z0i,P0i7,axis = 0)
                #print("Jdown is",Jup)
                Jdown = Z0i
                if i == 0:
                    J = np.concatenate((Jup,Jdown),axis = 0) #la premiere colonnne de la Jacobienne
                    """ print(P0i6)
                    print(Z0i)
                    print(Z0i[1]*P0i6[2]-Z0i[2]*P0i6[1])
                    print(T0i) """
                else:
                    J_i = np.concatenate((Jup,Jdown),axis = 0)
                    #print("le", i, "eme J_i est",J_i)
                    J = np.concatenate((J,J_i),axis = 1)
                    #print("le J est",J)
                    
            #print("\nthe transformation matrix is",T06)
            print("\nthe Jacobian matrix is",J)
            #print("\nthe position of the end of the robot is",T07)
            
            return T06, J

        def update(T):
            T07 = T @ O67
            X = T07[0:3].reshape(3,1)
            
            R = T[0:3,0:3]
            return X,R

        def initiation(T_i,T_f):
            X_i,R_i = update(T_i)
            X_f,R_f = update(T_f)
            R_i_inv = np.linalg.inv(R_i)
            R = R_i_inv @ R_f

            theta = np.arccos((R[0][0] + R[1][1] + R[2][2] - 1)/2)
            a = np.array([[R[2][1]-R[1][2]],[R[0][2]-R[2][0]],[R[1][0]-R[0][1]]])
            
            r = 1/(2*np.sin(theta))*a
            
            l = r[0]**2+r[1]**2+r[2]**2
            print("l est",l)
            return X_i,R_i,r,l,theta,X_f

        def GenerationTrajectoire(t,l,X_i,R_i,R,theta_angle,X_f):
            r = 10 * (t / tf) ** 3 - 15 * ( t / tf) ** 4 + 6 * (t / tf) ** 5  # Polynome de degré 5 pour la trajectoire
            r_dot = 30 * t ** 2 / tf ** 3 - 60 * t ** 3 / tf ** 4 + 30 * t ** 4 / tf ** 5
            x_desire = X_i + r * (X_f - X_i)
            v_desire = (X_f - X_i) * r_dot
           
            """ print("u_x est",X_i[0]/l) """
            """ print("1 = ",pow(X_i[0]/l,2)+pow(X_i[1]/l,2)+pow(X_i[2]/l,2)) """
            
            rot_matrix = np.array([[pow(R[0]/l,2)*(1-np.cos(r*theta_angle)) + np.cos(r*theta_angle), R[0]/l*R[1]/l*(1-np.cos(r*theta_angle)) - R[2]/l*np.sin(r*theta_angle), R[0]/l*R[2]/l*(1-np.cos(r*theta_angle)) + R[1]/l*np.sin(r*theta_angle)],
                        [R[0]/l*R[1]/l*(1-np.cos(r*theta_angle)) + R[2]/l*np.sin(r*theta_angle), pow(R[1]/l,2)*(1-np.cos(r*theta_angle)) + np.cos(r*theta_angle), R[1]/l*R[2]/l*(1-np.cos(r*theta_angle)) - R[0]/l*np.sin(r*theta_angle)],
                        [R[0]/l*R[2]/l*(1-np.cos(r*theta_angle)) - R[1]/l*np.sin(r*theta_angle), R[1]/l*R[2]/l*(1-np.cos(r*theta_angle)) + R[0]/l*np.sin(r*theta_angle), pow(R[2]/l,2)*(1-np.cos(r*theta_angle)) + np.cos(r*theta_angle)]])
            r_desire = R_i @ rot_matrix
            
            #print("rot_matrix is",rot_matrix)
            #print("size of x_d is", np.size(x_desire))
            #print("size of r_d is", np.size(r_desire))
            #print("R_d is",r_desire)
            return x_desire,r_desire,v_desire,r_dot


        def erreur(T06,X_present,x_desire,r_desire):
            e_p = x_desire - X_present


            n_d = r_desire[0:3,0].reshape(3,1)            
            s_d = r_desire[0:3,1].reshape(3,1)
            a_d = r_desire[0:3,2].reshape(3,1)

            n_e = T06[0:3,0].reshape(3,1)
            s_e = T06[0:3,1].reshape(3,1)
            a_e = T06[0:3,2].reshape(3,1)

            e_o = 1/2*(np.cross(n_e,n_d,axis=0) + np.cross(s_e,s_d,axis=0) + np.cross(a_e,a_d,axis=0)) #which tells cross that the vectors are defined along the first axis, rather than the last axis.


            L = -1/2 * (S(n_d)*S(n_e)+S(s_d)*S(s_e)+S(a_d)*S(a_e))
            return e_p,e_o,L


        def S(n):
            matrix_s = np.matrix([
                [0, -n[2][0], n[1][0]],
                [n[2][0], 0, -n[0][0]],
                [-n[1][0],n[0][0],0]
            ])
            
            return matrix_s

        def boucle():
            #initialize the time
            t = 0
            i = 0
            joint_pos_i = np.array([0.,np.pi/2,-np.pi/2,-np.pi/2,np.pi/4,0.])
            
            SetPos(clientID,h,joint_pos_i)
            time.sleep(1)
            T_i,J = Kinematics(joint_pos_i) #6x6
            T_f,J_f = Kinematics(A_f)
            X_i,R_i,r,l,theta,X_f = initiation(T_i,T_f)
            
            X_present = X_i
            desired_jointpos = joint_pos_i

            while t < tf :
                jointpos =  GetPos(clientID,h,sim.simx_opmode_blocking).reshape(6,1) #sim_joint a 6 articulaires
                joint_positions_all[:, i] = jointpos.flatten()  # Flatten to store 1D array

                #boucle
                print("\nmaintenant les positions des articulaires sont",jointpos)
                
                # Generate desired trajectory and velocities
                x_desire,r_desire,v_desire,r_dot = GenerationTrajectoire(t,l,X_i,R_i,r,theta,X_f)
                T06_, J = Kinematics(jointpos.flatten())
                X_present, R_present = update(T06_)
                print("la position de l'organe terminal est",X_present)
                end_effector_positions_all[:, i] = X_present.flatten()  # Store the position of the end effector

                # Semble pas avoir de probleme
                e_p, e_o,L = erreur(T06_,X_present,x_desire,r_desire)
                #print("e_p est",e_p)
                #print("e_o est",e_o)
                dX_d = K_p @ e_p # Desired velocity in Cartesian space
                dO_d = K_o @ e_o # Desired angular velocity in Cartesian space


                """ print("\nr_dot :",r_dot)
                print("\nR_i :",R_i)
                print("\nthera :",theta)
                print("\nr :",r) """
                w_d  = R_i * r_dot * theta @ r
                interm = L.T @ w_d
                """ print("Intermediate shape:", interm.shape)
                print("dO_d shape:", dO_d.shape)
                print("L shape:", L.shape)
                print("v_desire shape:", v_desire.shape)
                print("dX_d:", dX_d.shape) """
                v_dot = np.vstack([v_desire + dX_d,
                                   np.linalg.pinv(L) @ (interm + dO_d)
                                ])
                end_effector_velocities_all[:, i] = v_dot[0:3, 0].flatten()  # Store the desired velocity of the end effector
                print("v_dot:", v_dot.shape)
                J_inv = np.linalg.pinv(J)
                
                V_q = J_inv @ v_dot ## Joint velocities
                #print("V_q est", V_q*dt)
                desired_jointpos = V_q * dt + desired_jointpos
                print("\nles positions desiree des articulaires sont",desired_jointpos)
                SetPos(clientID,h,desired_jointpos)

                
                
                time.sleep(dt)
                t += dt
                i += 1
                print("time est",t)
            
            # Plotting the results
            time_values = np.linspace(0, tf, int(tf/dt) + 1)

            # Plot joint positions
            plt.figure(figsize=(10, 6))
            for i in range(6):
                plt.plot(time_values, joint_positions_all[i, :], label=f'Joint {i + 1}')
            plt.title("Joint Positions of UR10 Robot")
            plt.xlabel("Time [s]")
            plt.ylabel("Joint Position [rad]")
            plt.legend()
            plt.grid()
            plt.show()

            # Plot end effector position (X, Y, Z)
            plt.figure(figsize=(10, 6))
            plt.plot(time_values, end_effector_positions_all[0, :], label='X')
            plt.plot(time_values, end_effector_positions_all[1, :], label='Y')
            plt.plot(time_values, end_effector_positions_all[2, :], label='Z')
            plt.title("End Effector Position of UR10 Robot")
            plt.xlabel("Time [s]")
            plt.ylabel("Position [m]")
            plt.legend()
            plt.grid()
            plt.show()

            # Plot end effector velocity (vx, vy, vz)
            plt.figure(figsize=(10, 6))
            plt.plot(time_values, end_effector_velocities_all[0, :], label='Velocity X')
            plt.plot(time_values, end_effector_velocities_all[1, :], label='Velocity Y')
            plt.plot(time_values, end_effector_velocities_all[2, :], label='Velocity Z')
            plt.title("End Effector Velocity of UR10 Robot")
            plt.xlabel("Time [s]")
            plt.ylabel("Velocity [m/s]")
            plt.legend()
            plt.grid()
            plt.show()



        # call the function
        print("début de la simulation")
        time.sleep(2)
        boucle()
        
        # Before closing the connection to CoppeliaSim, make sure that the last command sent out had time to arrive. You can guarantee this with (for example):
        sim.simxGetPingTime(clientID)

        # Now close the connection to CoppeliaSim:
        sim.simxFinish(clientID)
    else:
        print ('Failed connecting to remote API server')
    print ('Program ended')


