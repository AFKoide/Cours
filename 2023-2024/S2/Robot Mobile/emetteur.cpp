#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "geometry_msgs/msg/point.hpp"

#include <opencv2/opencv.hpp>


class SenderNode : public rclcpp::Node
{
private:
// ROS2
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_publisher_;
    rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr point_publisher_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr pairing_publisher_;

    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr pairing_subscriber_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr recording_subscriber_;
    rclcpp::
// Class Variable
    bool receiver_ready = false;
    
    std::vector<cv::Mat> images;                    unsigned int image_count;
    std::vector<geometry_msgs::msg::Point> points;  unsigned int point_count;

public:
    SenderNode() : Node("sender_node")
    {
        image_publisher_ = this->create_publisher<sensor_msgs::msg::Image>("image_topic", 1);
        point_publisher_ = this->create_publisher<geometry_msgs::msg::Point>("point_topic", 1);
        
        pairing_publisher_ = this->create_publisher<std_msgs::msg::Bool>("pairing_topic", 1);
        pairing_subscriber_ = this->create_subscription<std_msgs::msg::Bool>("pairing_topic", 1, std::bind(&SenderNode::pairing_callback, this, std::placeholders::_1));

        recording_subscriber_ = this->crete_subscriber<std_msgs::msg::Bool>("recording_active", 1, std::bind(&SenderNode::recording_callback, this, std::placeholders::_1))

        image_count = 0;
        point_count = 0;


        auto message = std_msgs::msg::Bool();
        message.data = true;
        pairing_publisher_->publish(message);
    }

    ~SenderNode() {
        images.clear();
    }

private:
    void pairing_callback(const std_msgs::msg::Bool::SharedPtr msg)
    {
        // Pong : The receiver is ready to collect the informations.
        if(msg && !receiver_ready) {
            RCLCPP_INFO(this->get_logger(), "Sender ready.");

            // Flag up : ready to receive the data.
            receiver_ready = true;
        }
    }

    void run()
    {
/* TODO: 
- Launch iai_kinect2
- Start a recording using kinect2_record (with parameters declared when starting this node)
- Send the images recorded by iai into openpose
- Collect openpose output (image+points) and send it to the receiver

The node should start recording only when the receiver is ready.
*/
    }

    void OpenMove()
    {

    }
};




int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<SenderNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
}
