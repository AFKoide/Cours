#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "geometry_msgs/msg/point.hpp"


#include <opencv2/opencv.hpp>


class ReceiverNode : public rclcpp::Node
{
private:
// ROS2


// Class Variable

public:
    ReceiverNode() : Node("receiver_node")
    {
        
    }

    ~ReceiverNode() {
    }

private:

};




int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<ReceiverNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
}
