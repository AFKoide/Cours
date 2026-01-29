#include <chrono> // Date and time
#include <functional> // Arithmetic, comparisons, and logical operations
#include <memory> // Dynamic memory management
#include <string> // String functions
#include "eigen3/Eigen/Dense"
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/bool.hpp"
#include "ros2_planar_robot/msg/ref_pose.hpp"
#include "ros2_planar_robot/msg/kin_data.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;
 
 // High level manager node
class HighLevMng : public rclcpp::Node
{
  public:
    HighLevMng():
      Node("high_lev_mng")
      {      
        // Get the sequence of desired poses to visit, storing the data in MotionData
        std::cout << "Enter the number of poses: ";
        std::cin >> n_;
        MotionData.resize(3,n_);        
        for(int i = 0; i < n_; i++){
          std::cout << "======================================\n";
          std::cout << "Enter " << i+1 << "th pose X      : ";
          std::cin >> MotionData(0,i);
          std::cout << "Enter " << i+1 << "th pose Y      : ";
          std::cin >> MotionData(1,i);
          std::cout << "Enter " << i+1 << "th pose Delta t: ";
          std::cin >> MotionData(2,i);
          std::cout << "======================================\n";
        }

        // Publisher to the next reference pose, to be sent when the previous one is achieved
        pub_nxt_pose_ = this->create_publisher    <ros2_planar_robot::msg::RefPose>(
            "nxt_pose",10
        );
        // Subscription to read the current pose (used to define when nxt pose should be sent)
        subs_kin_data_ = this->create_subscription <ros2_planar_robot::msg::KinData>   (
            "kin_data", 10, std::bind(&HighLevMng::topic_callback, this, _1)
        );
        // Subscription to the launch node, used to launch the whole system
        subs_launch_  = this->create_subscription <std_msgs::msg::Bool>   (
            "launch", 10, std::bind(&HighLevMng::launch_callback, this, _1)
        );
      }
 
  private:
    // Callback used to define if the nxt pose should be sent, based on the current pose
    void topic_callback(const ros2_planar_robot::msg::KinData::SharedPtr kin_data)    {
      x_current_    << kin_data->pose.x, kin_data->pose.y;                      //  Read current pose
      if(!running_){                                                            //  If not running, the read pose is the initial one. It is sent to initialize the trajectory generator
        running_        = true;
        x_nxt_          << kin_data->pose.x, kin_data->pose.y;
        auto nxt_pose   = ros2_planar_robot::msg::RefPose();
        nxt_pose.x      = x_nxt_(0);
        nxt_pose.y      = x_nxt_(1);
        nxt_pose.deltat = 0;
        pub_nxt_pose_->publish(nxt_pose);  
      }
      else if(!awaiting_ && i_ < n_){                                           // If running and not waiting (launch flag has been sent) check if current pose is equal to the next one
        if((x_current_-x_nxt_).norm() < eps_){          
          x_nxt_    << MotionData(0,i_), MotionData(1,i_);
          Dt        = MotionData(2,i_);
          i_++;
          auto nxt_pose   = ros2_planar_robot::msg::RefPose();
          nxt_pose.x      = x_nxt_(0);
          nxt_pose.y      = x_nxt_(1);
          nxt_pose.deltat = Dt;
          pub_nxt_pose_->publish(nxt_pose);      
        }
      }
    }
    // Launch the whole process when the flag is received
    void launch_callback(const std_msgs::msg::Bool::SharedPtr launch)    {
      if (launch->data && awaiting_)
      {
        awaiting_   = false;
        auto nxt_pose   = ros2_planar_robot::msg::RefPose();
        x_nxt_          << MotionData(0,0),MotionData(1,0);
        nxt_pose.x      = MotionData(0,0);
        nxt_pose.y      = MotionData(1,0);
        nxt_pose.deltat = MotionData(2,0);
        pub_nxt_pose_->publish(nxt_pose);                                       // Publish first desired pose
        i_++;    
      }      
    }
    rclcpp::TimerBase                                     ::SharedPtr   timer_          ;
    rclcpp::Publisher<ros2_planar_robot::msg::RefPose>    ::SharedPtr  pub_nxt_pose_    ;       
    rclcpp::Subscription<ros2_planar_robot::msg::KinData> ::SharedPtr  subs_kin_data_   ;        
    rclcpp::Subscription<std_msgs::msg::Bool>             ::SharedPtr  subs_launch_     ;        
    bool          running_  {false};
    bool          awaiting_ {true};
    double        count_    {0};
    double        Dt        {0};
    int           n_        {0};
    int           i_        {0};
    int           current_  {0};
    const double  eps_      {1e-3};
    Eigen::Vector2d   x_current_  {Eigen::Vector2d::Zero()};
    Eigen::Vector2d   x_nxt_      {Eigen::Vector2d::Zero()};
    Eigen::MatrixXd   MotionData  {Eigen::MatrixXd(3,20)  };    
};
 
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<HighLevMng>());  
  rclcpp::shutdown();
  return 0;
}
