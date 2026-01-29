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

// Controller node, computing the feedback desired joint velocity
class ControlNode : public rclcpp::Node
{
  public:
    ControlNode():
      Node("controller")
      {      
        // Publisher of the feedback desired joint velocity
        pub_desired_velocity_ = this->create_publisher    <ros2_planar_robot::msg::JointVelocity>(
            "Dqd",10
        );
        // Subscriber of the desired cartesian states (cartesian position and velocity)
        subs_desired_state_   = this->create_subscription <ros2_planar_robot::msg::CartesianState>   (
            "desired_state", 10, std::bind(&ControlNode::desired_state_callback, this, _1)
        );
        // Subscriber of the kinematic data topic (cartesian position and jacobian matrix), obtained with the kinematic model
        subs_kin_data_  = this->create_subscription <ros2_planar_robot::msg::KinData>   (
            "kin_data", 10, std::bind(&ControlNode::kin_data_callback, this, _1)
        );
        // Timer callback streaming the desired joint velocity
        timer_ = this->create_wall_timer(
          1ms, std::bind(&ControlNode::timer_callback, this));
      }      
 
  private:
    // Timer callback streaming the desired joint velocity
    void timer_callback(){
      auto  Dqd_message = ros2_planar_robot::msg::JointVelocity();
      if (running_){                                        // If running, compute the feedback velocity
        Dqd_   = cnt.Dqd(x_,xd_,Dxff_,J_);
      }                                                     // Otherwise, stay still
      else{
        Dqd_   << 0, 0;
      }
      Dqd_message.dq = {Dqd_(0), Dqd_(1)};
      pub_desired_velocity_->publish(Dqd_message);          // Publishes the array
    }

    // Callback performed when something is published in the /desired_state topic
    void desired_state_callback(const ros2_planar_robot::msg::CartesianState::SharedPtr desired_state)    {
      if (!running_){                                                             // Set running_ = true
        running_  = true;
      }
      xd_   << desired_state->pose      .x ,       desired_state->pose     .y;    // Update internal variables
      Dxff_ << desired_state->velocity  .dx,       desired_state->velocity .dy;      
    }

    // Update internal variables based on kinematic model 
    void kin_data_callback(const ros2_planar_robot::msg::KinData::SharedPtr kin_data)    {
      J_  << kin_data->jacobian[0], kin_data->jacobian[1], kin_data->jacobian[2], kin_data->jacobian[3];
      x_  << kin_data->pose.x, kin_data->pose.y;
    }
    rclcpp::TimerBase                                           ::SharedPtr timer_                   ;
    rclcpp::Publisher<ros2_planar_robot::msg::JointVelocity>    ::SharedPtr pub_desired_velocity_    ;   
    rclcpp::Subscription<ros2_planar_robot::msg::CartesianState>::SharedPtr subs_desired_state_      ;        
    rclcpp::Subscription<ros2_planar_robot::msg::KinData>       ::SharedPtr subs_kin_data_           ;        
    bool                running_  { false                   } ; 
    Eigen::Vector2d     xd_       { Eigen::Vector2d::Zero() } ;
    Eigen::Vector2d     x_        { Eigen::Vector2d::Zero() } ;
    Eigen::Vector2d     Dxff_     { Eigen::Vector2d::Zero() } ;
    Eigen::Vector2d     Dqd_      { Eigen::Vector2d::Zero() } ;
    Eigen::Matrix2d     J_        { Eigen::Matrix2d::Zero() } ;
    Controller          cnt                                   ;
};
 
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ControlNode>());  
  rclcpp::shutdown();
  return 0;
}
