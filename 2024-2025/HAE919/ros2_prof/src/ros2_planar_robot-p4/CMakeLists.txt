cmake_minimum_required(VERSION 3.8)
project(ros2_planar_robot)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package  ( ament_cmake               REQUIRED)
find_package  ( geometry_msgs             REQUIRED)
find_package  ( sensor_msgs               REQUIRED)
find_package  ( rclcpp                    REQUIRED)
find_package  ( Eigen3 3.4                REQUIRED)
find_package  ( rosidl_default_generators REQUIRED)

# Header files should be placed in include folder
include_directories         ( include )

# msg files
set(msg_files
  "msg/RefPose.msg"
  "msg/Pose.msg"
  "msg/JointPosition.msg"
  "msg/JointVelocity.msg"
  "msg/CartesianState.msg"
  "msg/Velocity.msg"
  "msg/KinData.msg"
)

# Generate interfaces (services and messages) to be used in the same package
#   targets to be used as "${cpp_typesupport_target}"
rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  # ${srv_files}
)

ament_export_dependencies(rosidl_default_runtime)
rosidl_get_typesupport_target(cpp_typesupport_target
  ${PROJECT_NAME} rosidl_typesupport_cpp)

# Create libraries
add_library                 ( trajectory_generation_lib src/trajectory_generation_lib.cpp )
target_link_libraries       ( trajectory_generation_lib Eigen3::Eigen                     )
add_library                 ( kinematic_model_lib       src/kinematic_model_lib.cpp       )
target_link_libraries       ( kinematic_model_lib       Eigen3::Eigen                     )
add_library                 ( control_lib               src/control_lib.cpp               )
target_link_libraries       ( control_lib               Eigen3::Eigen kinematic_model_lib )

# Create executables
add_executable              ( high_lev_mng 
                              src/high_lev_mng.cpp)
ament_target_dependencies   ( high_lev_mng  
                              rclcpp geometry_msgs sensor_msgs std_msgs )          
target_link_libraries       ( high_lev_mng 
                              "${cpp_typesupport_target}" )                                
  
add_executable              ( trajectory_generator 
                                src/trajectory_generator.cpp)
ament_target_dependencies   ( trajectory_generator 
                                rclcpp geometry_msgs sensor_msgs std_msgs)
target_link_libraries       ( trajectory_generator 
                                trajectory_generation_lib "${cpp_typesupport_target}" )

add_executable              ( kinematic_modeller 
                                src/kinematic_modeller.cpp)
ament_target_dependencies   ( kinematic_modeller 
                                rclcpp geometry_msgs sensor_msgs std_msgs)
target_link_libraries       ( kinematic_modeller 
                                "${cpp_typesupport_target}" kinematic_model_lib )

add_executable              ( controller 
                                src/controller.cpp)
ament_target_dependencies   ( controller 
                                rclcpp geometry_msgs sensor_msgs std_msgs)
target_link_libraries       ( controller 
                                "${cpp_typesupport_target}" control_lib )

add_executable              ( simulator 
                                src/simulator.cpp)
ament_target_dependencies   ( simulator 
                                rclcpp geometry_msgs sensor_msgs std_msgs)
target_link_libraries       ( simulator 
                                "${cpp_typesupport_target}" )

add_executable              ( disturbance_generator 
                                src/disturbance.cpp)
ament_target_dependencies   ( disturbance_generator 
                                rclcpp geometry_msgs sensor_msgs std_msgs)
target_link_libraries       ( disturbance_generator 
                                "${cpp_typesupport_target}" )

# Add joint_pub executable
add_executable              ( joint_pub 
                                src/joint_pub.cpp)
ament_target_dependencies   ( joint_pub 
                                rclcpp sensor_msgs )
target_link_libraries       ( joint_pub 
                                "${cpp_typesupport_target}" )

# Install executables
install(TARGETS
  trajectory_generator
  high_lev_mng
  controller
  kinematic_modeller
  simulator
  disturbance_generator
  joint_pub
  DESTINATION lib/${PROJECT_NAME}
)
                     
# Install libraries
install(TARGETS
  trajectory_generation_lib
  control_lib
  kinematic_model_lib
  DESTINATION lib
)

# Install launch files.
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

# Install urdf files.
install(DIRECTORY
  urdf
  DESTINATION share/${PROJECT_NAME}/
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
