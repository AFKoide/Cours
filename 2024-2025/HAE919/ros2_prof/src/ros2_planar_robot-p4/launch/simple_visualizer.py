import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    # Get the package directory
    package_dir = get_package_share_directory('ros2_planar_robot')

    # URDF file
    urdf_file = os.path.join(package_dir, 'urdf', 'planar_robot.urdf.xml')
    with open(urdf_file, 'r') as infp:
        robot_desc = infp.read()

    # RViz config file
    rviz_config_file = os.path.join(package_dir, 'urdf', 'planar_robot.rviz')

    return LaunchDescription([
        # Static transform broadcaster
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='static_transform_publisher',
            arguments=["0", "0", "0", "0", "0", "0", "odom", "axis"]
        ),

        # Robot state publisher
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            name='robot_state_publisher',
            parameters=[{'robot_description': robot_desc}],
        ),

        # Launch the Joint Publisher node 
        Node(
            package='ros2_planar_robot',
            executable='joint_pub',
            name='joint_pub',
            output='screen',
        ),

        # Launch RViz
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            arguments=['-d', rviz_config_file],
        ),
    ])
