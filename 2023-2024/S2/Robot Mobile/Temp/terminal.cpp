#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "geometry_msgs/msg/point.hpp"

#include <opencv2/opencv.hpp>

class Terminal_Node : public rclcpp::Node
{
private:
    // ROS
    rlcpp::Subscriber<sensor_msgs::msg::Image>::SharedPtr image_subs_;
    rlcpp::Subscriber<geometry_msgs::msg::Point>::SharedPtr 


    // VARIABLES

public:

private:
}