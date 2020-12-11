#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4456)
#endif
#include <rclcpp/rclcpp.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <ecal/ecal.h>
#include <ecal/msg/protobuf/publisher.h>

#include <functional>
#include <memory>

#include <sensor_msgs/msg/nav_sat_fix.hpp>
#include <sensor_msgs/msg/temperature.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251)
#endif
#include "sensor_msgs/navsatfix.pb.h"
#include "sensor_msgs/temperature.pb.h"
#ifdef _MSC_VER
#pragma warning(pop)
#endif

class GatewayNode : public rclcpp::Node
{
public:
  GatewayNode() : Node("Ros2eCAL")
  {
    // initialize pub/sub for <sensor_msgs::msg::NavSatFix>
    pub_nav_ = eCAL::protobuf::CPublisher<pb::NavSatFix>("navsatfix");
    sub_nav_ = this->create_subscription<sensor_msgs::msg::NavSatFix>("navsatfix", 10, std::bind(&GatewayNode::navsatfix_cb, this, std::placeholders::_1));
  }

private:
  void navsatfix_cb(const sensor_msgs::msg::NavSatFix::SharedPtr msg) //-V801
  {
    // clear proto message
    msg_nav_.Clear();

    // header
    //auto header = msg_nav_.mutable_header();

    //msg_nav_.mutable_status()->set_status(static_cast<google::protobuf::uint32>(msg->status.status));

    // send it to eCAL
    pub_nav_.Send(msg_nav_);
  }

  rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr  sub_nav_;
  pb::NavSatFix                                                 msg_nav_;
  eCAL::protobuf::CPublisher<pb::NavSatFix>                     pub_nav_;
};


int main(int argc, char* argv[])
{
  // initialize eCAL and set process state
  eCAL::Initialize(argc, argv, "Ros2eCAL");
  eCAL::Process::SetState(proc_sev_healthy, proc_sev_level1, "I'm fine ..");

  // initialize ROS node
  rclcpp::init(argc, argv);

  // start ROS node
  auto node = std::make_shared<GatewayNode>();
  rclcpp::spin(node);

  // shutdown ROS node
  rclcpp::shutdown();

  // finalize eCAL
  eCAL::Finalize();

  return 0;
}
