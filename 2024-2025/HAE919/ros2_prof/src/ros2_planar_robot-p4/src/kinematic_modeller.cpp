#include <chrono> // Date and time
#include <functional> // Arithmetic, comparisons, and logical operations
#include <memory> // Dynamic memory management
#include <string> // String functions
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "kinematic_model_lib.h"
#include "ros2_planar_robot/msg/joint_position.hpp"
#include "ros2_planar_robot/msg/kin_data.hpp"
#include "ros2_planar_robot/msg/pose.hpp"
#include "ros2_planar_robot/msg/cartesian_state.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

// Node able to compute cartesian position and jacobian matrix for a given joint position 
class KinematicModel : public rclcpp::Node
{
  public:
    KinematicModel():
      Node("kinematic_model")
      {      
        // Declare lengths l1 and l2 as node parameters
        this->declare_parameter("l1", 0.0);
        this->declare_parameter("l2", 0.0);     
        // Publisher in the topic /kin_data, containing cartesian position and jacobian matrix
        pub_kin_data_ = this->create_publisher    <ros2_planar_robot::msg::KinData>(
            "kin_data",10
        );
        // Subscriber of the joint position topic, taking q as input
        subs_joint_position_ = this->create_subscription <ros2_planar_robot::msg::JointPosition>   (
            "q", 10, std::bind(&KinematicModel::topic_callback, this, _1)
        );
      }
 
  private:
    // Robot model pointer, used to perform the computation
    RobotModel * rm;

    void topic_callback(const ros2_planar_robot::msg::JointPosition::SharedPtr joint_position)    {
      auto kin_data = ros2_planar_robot::msg::KinData();
      if (!running_)                                            // If not initialized, a RobotModel should be instantiated
      {
        double l1 = this->get_parameter("l1").as_double();      // Retrieve node parameters
        double l2 = this->get_parameter("l2").as_double();
        if(l1 > 0 && l2 > 0){                                   // If the parameters were not initialized, nothing is done
          rm  = new RobotModel(l1,l2);                          //    Otherwise the model is instantiated
          running_  = true;                                     //    and the flag is set to running_ = true
        }
      }
      if(running_){                                             // If the node is already operational, simply compute x, J and publish
        q_ << joint_position->q[0], joint_position->q[1];
        rm->FwdKin(x_,J_,q_);
        kin_data.jacobian[0] = J_(0,0);
        kin_data.jacobian[1] = J_(0,1);
        kin_data.jacobian[2] = J_(1,0);
        kin_data.jacobian[3] = J_(1,1);
        kin_data.pose.x      = x_(0);
        kin_data.pose.y      = x_(1);
        pub_kin_data_       ->publish(kin_data);
      }
    }
    rclcpp::Publisher<ros2_planar_robot::msg::KinData>          ::SharedPtr  pub_kin_data_          ;   
    rclcpp::Subscription<ros2_planar_robot::msg::JointPosition> ::SharedPtr  subs_joint_position_   ;        
    bool                running_  { false                   } ;
    Eigen::Vector2d     x_        { Eigen::Vector2d::Zero() } ;
    Eigen::Vector2d     q_        { Eigen::Vector2d::Zero() } ;
    Eigen::Matrix2d     J_        { Eigen::Matrix2d::Zero() } ;
     
};
 
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);  
  rclcpp::spin(std::make_shared<KinematicModel>());
  rclcpp::shutdown();
  return 0;
}
