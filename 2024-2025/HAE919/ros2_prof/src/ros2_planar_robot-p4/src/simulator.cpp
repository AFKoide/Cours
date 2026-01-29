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

// Simulator node, computing next joint position based on the desired joint velocity, disturbance and previous joint position
class Simulator : public rclcpp::Node
{
  public:
    Simulator():
      Node("sim")
      {      
        // Publishes the next joint position
        pub_joint_position_ = this->create_publisher    <ros2_planar_robot::msg::JointPosition>(
            "q",10
        );
        // Reads the desired joint velocity
        sub_desired_velocity_ = this->create_subscription    <ros2_planar_robot::msg::JointVelocity>(
            "Dqd",10, std::bind(&Simulator::receive_Dqd, this, _1)
        );
        // Reads the disturbances
        sub_disturbance_      = this->create_subscription    <ros2_planar_robot::msg::JointVelocity>(
            "Dq_dist",10, std::bind(&Simulator::receive_Dq_dist, this, _1)
        );       
        // Streams the next joint positions
        timer_ = this->create_wall_timer(
          1ms, std::bind(&Simulator::timer_callback, this));
      }      
 
  private:
    // Streams the next joint positions
    void timer_callback(){
      auto  joint_position = ros2_planar_robot::msg::JointPosition();
      q_    = (Dqd_+Dq_dist_)*dt_ + q_;                                     // Integrates in time the desired joint velocity and the disturbance
      joint_position.q = {q_(0), q_(1)};
      pub_joint_position_->publish(joint_position);                         // Publishes next joint position
    }

    // Update internal variable desired joint velocity
    void receive_Dqd(const ros2_planar_robot::msg::JointVelocity::SharedPtr desired_velocity)    {
      Dqd_  << desired_velocity->dq[0],       desired_velocity->dq[1];
    }

    // Update internal variable disturbance
    void receive_Dq_dist(const ros2_planar_robot::msg::JointVelocity::SharedPtr disturbance)    {
      Dq_dist_  << disturbance->dq[0],       disturbance->dq[1];
    }
    rclcpp::TimerBase                                           ::SharedPtr timer_                  ;
    rclcpp::Publisher<ros2_planar_robot::msg::JointPosition>    ::SharedPtr pub_joint_position_     ;   
    rclcpp::Subscription<ros2_planar_robot::msg::JointVelocity> ::SharedPtr sub_desired_velocity_   ;        
    rclcpp::Subscription<ros2_planar_robot::msg::JointVelocity> ::SharedPtr sub_disturbance_        ;        
    bool                running_  { false                           } ; 
    const double        dt_       { 1e-3                            } ;
    Eigen::Vector2d     q_        { Eigen::Vector2d{3.04,1.66}      } ;
    Eigen::Vector2d     x_        { Eigen::Vector2d::Zero()         } ;
    Eigen::Vector2d     Dxff_     { Eigen::Vector2d::Zero()         } ;
    Eigen::Vector2d     Dqd_      { Eigen::Vector2d::Zero()         } ;
    Eigen::Vector2d     Dq_dist_  { Eigen::Vector2d::Zero()         } ;
    Eigen::Vector2d     J_        { Eigen::Vector2d::Zero()         } ;
};
 
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Simulator>());  
  rclcpp::shutdown();
  return 0;
}
