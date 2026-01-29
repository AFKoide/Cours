#include <chrono> // Date and time
#include <functional> // Arithmetic, comparisons, and logical operations
#include <memory> // Dynamic memory management
#include <string> // String functions
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "trajectory_generation_lib.h"
#include "ros2_planar_robot/msg/ref_pose.hpp"
#include "ros2_planar_robot/msg/pose.hpp"
#include "ros2_planar_robot/msg/cartesian_state.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

// Trajectory generator node 
class TrajGen : public rclcpp::Node
{
  public:
    TrajGen():
      Node("traj_gen")
      {      
        // Publisher on current desired state (to be used for each milisecond)
        pub_desired_pose_ = this->create_publisher    <ros2_planar_robot::msg::CartesianState>(
            "desired_state",10
        );
        // Subscribe on nxt_pose topic, managed by high_lev_mng node
        subs_next_pose_   = this->create_subscription <ros2_planar_robot::msg::RefPose>   (
            "nxt_pose", 10, std::bind(&TrajGen::topic_callback, this, _1)
        );
      }
 
  private:
    // Publishes current desired pose depending on the states awaiting 
    void timer_callback(){
      t   += dt;
      auto state = ros2_planar_robot::msg::CartesianState();
      if(awaiting_){
        state.pose    .x        = x_next_(0);
        state.pose    .y        = x_next_(1);
        state.velocity.dx       = 0;
        state.velocity.dy       = 0;
      }else{          
        Eigen::Vector2d x   = p2p.X(t);
        Eigen::Vector2d Dx  = p2p.dX(t);
        if (t > Dt){
          state.pose    .x        = x_next_(0);
          state.pose    .y        = x_next_(1);
          state.velocity.dx       = 0;
          state.velocity.dy       = 0;          
        }else{
          x   = p2p.X(t);
          Dx  = p2p.dX(t);
          state.pose    .x    = x(0);
          state.pose    .y    = x(1);
          state.velocity.dx   = Dx(0);
          state.velocity.dy   = Dx(1);        
        }
      }
      pub_desired_pose_->publish(state);
    }

    // Update polynomials and states based on next pose
    void topic_callback(const ros2_planar_robot::msg::RefPose::SharedPtr ref_pose)    {
      x_previous_ = x_next_;
      x_next_     << ref_pose->x, ref_pose->y ;
      Dt          = ref_pose->deltat         ;
      if (!running_)
      {
        timer_ = this->create_wall_timer(
          1ms, std::bind(&TrajGen::timer_callback, this));        
        awaiting_ = true;
        running_  = true;        
      }
      else {
        p2p.update(x_previous_,x_next_,Dt);
        t         = 0;
        awaiting_ = false;
      }    
    }
    
    Point2Point p2p{Eigen::Vector2d::Zero(),Eigen::Vector2d(1,1),3};
    bool          running_            { false                   } ;
    bool          awaiting_           { true                    } ;
    double        Dt                  { 0                       } ;
    const double  dt                  { 1e-3                    } ;
    double        t                   { 0                       } ;
    Eigen::Vector2d x_next_     { Eigen::Vector2d::Zero() } ;
    Eigen::Vector2d x_previous_ { Eigen::Vector2d::Zero() } ;
    rclcpp::TimerBase::SharedPtr                                          timer_                                          ;
    rclcpp::Publisher<ros2_planar_robot::msg::CartesianState>::SharedPtr  pub_desired_pose_                               ;   
    rclcpp::Subscription<ros2_planar_robot::msg::RefPose>::SharedPtr      subs_next_pose_                                 ;        
};
 
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TrajGen>());  
  rclcpp::shutdown();
  return 0;
}
