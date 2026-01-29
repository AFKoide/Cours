#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class SubscriberNode : public rclcpp::Node
{
public:
    SubscriberNode() : Node("time_subscriber")
    {
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "image_topic", 10, std::bind(&SubscriberNode::message_callback, this, std::placeholders::_1));
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "point_topic", 10, std::bind(&SubscriberNode::message_callback, this, std::placeholders::_1));
    }

private:
    void message_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Received message: '%s'", msg->data.c_str());
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SubscriberNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
