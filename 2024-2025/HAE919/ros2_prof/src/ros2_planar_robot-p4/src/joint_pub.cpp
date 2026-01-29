#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "ros2_planar_robot/msg/joint_position.hpp"

using namespace std::chrono_literals;

class JointPubNode : public rclcpp::Node
{
public:
    JointPubNode() : Node("joint_pub")
    {
        // Subscription to /q
        q_sub_ = this->create_subscription<ros2_planar_robot::msg::JointPosition>(
            "/q", 10, std::bind(&JointPubNode::q_callback, this, std::placeholders::_1));

        // Publisher to /joint_states
        joint_pub_ = this->create_publisher<sensor_msgs::msg::JointState>("/joint_states", 10);

        RCLCPP_INFO(this->get_logger(), "Joint publisher node initialized.");
    }

private:
    rclcpp::Subscription<ros2_planar_robot::msg::JointPosition>::SharedPtr q_sub_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pub_;

  void q_callback(const ros2_planar_robot::msg::JointPosition::SharedPtr msg)
  {
      // Create and populate the JointState message
      auto joint_state_msg = sensor_msgs::msg::JointState();
      joint_state_msg.header.stamp = this->get_clock()->now();
      joint_state_msg.name = {"r1", "r2"}; // Use the joint names from your URDF file
      joint_state_msg.position = {msg->q[0], msg->q[1]}; // Use 'q' instead of 'data'

      // Publish the JointState message
      joint_pub_->publish(joint_state_msg);

      // Log the published joint states
      RCLCPP_INFO(this->get_logger(), "Published joint states: [%f, %f]", msg->q[0], msg->q[1]);
  }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<JointPubNode>());
    rclcpp::shutdown();
    return 0;
}
