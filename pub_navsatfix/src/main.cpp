#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/nav_sat_fix.hpp>

using namespace std::chrono_literals;

class NavSatFixPublisher : public rclcpp::Node
{
public:
  NavSatFixPublisher() : Node("pub_navsatfix")
  {
    pub_ = this->create_publisher<sensor_msgs::msg::NavSatFix>("navsatfix", 10);
    msg_ = sensor_msgs::msg::NavSatFix();

    FillMessage(msg_);

    auto timer_callback =
      [this]() -> void {
        RCLCPP_INFO(this->get_logger(), "Publishing NavSatFix");
        this->pub_->publish(this->msg_);
      };

    timer_ = this->create_wall_timer(100ms, timer_callback);
  }

  void FillMessage(sensor_msgs::msg::NavSatFix& msg)
  {
    // header
    msg.header.stamp.sec     = 1;
    msg.header.stamp.nanosec = 2;
    msg.header.frame_id      = 3;

    // status
    msg.status.status = sensor_msgs::msg::NavSatStatus::STATUS_FIX;

    // latitude
    msg.latitude  = 0.1;

    // longitude
    msg.longitude = 0.2;

    // altitude
    msg.altitude  = 0.3;

    // position_covariance[]
    msg.position_covariance = { 1.0, 2.0, 3.0, 4.0, 5.0 };
  }

private:
  rclcpp::TimerBase::SharedPtr                               timer_;
  rclcpp::Publisher<sensor_msgs::msg::NavSatFix>::SharedPtr  pub_;
  sensor_msgs::msg::NavSatFix                                msg_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<NavSatFixPublisher>());
  rclcpp::shutdown();
  return 0;
}