import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # Launch ros2_planar_robot nodes
        Node(
            package='ros2_planar_robot',
            executable='controller',
            name='controller'
        ),
        Node(
            package='ros2_planar_robot',
            executable='simulator',
            name='simulator'
        ),
        Node(
            package='ros2_planar_robot',
            executable='trajectory_generator',
            name='trajectory_generator'
        ),
        Node(
            package='ros2_planar_robot',
            executable='kinematic_modeller',
            name='kinematic_model',            
            parameters=[
                {"l1": 1.0},
                {"l2": 1.0},
            ]
        ),
        Node(
            package='ros2_planar_robot',
            executable='disturbance_generator',
            name='disturbance_generator'
        )
    ])