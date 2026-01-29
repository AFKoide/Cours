// disturbance.cpp
#include <chrono> // Date and time
#include <functional> // Arithmetic, comparisons, and logical operations
#include <memory> // Dynamic memory management
#include <string> // String functions
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "control_lib.h"
#include "ros2_planar_robot/msg/pose.hpp"
#include "ros2_planar_robot/msg/joint_position.hpp"
#include "ros2_planar_robot/msg/joint_velocity.hpp"
#include "ros2_planar_robot/msg/cartesian_state.hpp"
#include "ros2_planar_robot/msg/kin_data.hpp"


using namespace std::chrono_literals;
using std::placeholders::_1;

// Disturbance node
class Disturbance : public rclcpp::Node
{
  public:
    Disturbance():
      Node("pert")
      {      
        // Publishes the disturbance
        this->declare_parameter("A", 0.4);
        this->declare_parameter("omega", 3.0);             
        pub_disturbance_ = this->create_publisher    <ros2_planar_robot::msg::JointVelocity>(
            "Dq_dist",10
        );
        // Streams it every 1 ms
        timer_ = this->create_wall_timer(
          1ms, std::bind(&Disturbance::timer_callback, this));
      }      
 
  private:
    void timer_callback(){
      time_ += 1e-3;
      double A      = this->get_parameter("A").as_double();      // Retrieve node parameters
      double omega  = this->get_parameter("omega").as_double();
      auto  disturbance = ros2_planar_robot::msg::JointVelocity();
      disturbance.dq = {A*sin(omega*time_),-A*sin(2*omega*time_)};
      pub_disturbance_->publish(disturbance);
    }
    rclcpp::TimerBase                                           ::SharedPtr timer_            ;
    rclcpp::Publisher<ros2_planar_robot::msg::JointVelocity>    ::SharedPtr pub_disturbance_  ;
    double                                                                  time_  {0}        ;
};
 
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Disturbance>());  
  rclcpp::shutdown();
  return 0;
}
