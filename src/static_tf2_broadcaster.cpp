#include <memory>
#include <string>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/static_transform_broadcaster.h"

class StaticFramePublisher : public rclcpp::Node
{
public:
  explicit StaticFramePublisher()
  : Node("static_tf2_broadcaster")
  {
    this->declare_parameter("parent_frame");
    this->declare_parameter("child_frame");
    this->declare_parameter("translation_x");
    this->declare_parameter("translation_y");
    this->declare_parameter("translation_z");
    this->declare_parameter("rotation_x");
    this->declare_parameter("rotation_y");
    this->declare_parameter("rotation_z");
    
    tf_static_broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);

    // Publish static transforms once at startup
    this->make_transforms();
  }

private:
  void make_transforms()
  {
    geometry_msgs::msg::TransformStamped t;
    std::string parent_frame_;
    std::string child_frame_;
    double translation_x_;
    double translation_y_;
    double translation_z_;
    double rotation_x_;
    double rotation_y_;
    double rotation_z_;

    this->get_parameter("parent_frame", parent_frame_);
    this->get_parameter("child_frame", child_frame_);
    this->get_parameter("translation_x", translation_x_);
    this->get_parameter("translation_y", translation_y_);
    this->get_parameter("translation_z", translation_z_);
    this->get_parameter("rotation_x", rotation_x_);
    this->get_parameter("rotation_x", rotation_y_);
    this->get_parameter("rotation_x", rotation_z_);


    t.header.stamp = this->get_clock()->now();
    t.header.frame_id = parent_frame_;
    t.child_frame_id = child_frame_;

    t.transform.translation.x = translation_x_;
    t.transform.translation.y = translation_y_;
    t.transform.translation.z = translation_z_;
    tf2::Quaternion q;
    q.setRPY(
      rotation_x_,
      rotation_y_,
      rotation_z_);
    t.transform.rotation.x = q.x();
    t.transform.rotation.y = q.y();
    t.transform.rotation.z = q.z();
    t.transform.rotation.w = q.w();

    tf_static_broadcaster_->sendTransform(t);
  }

  std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_static_broadcaster_;
};

int main(int argc, char * argv[])
{
  auto logger = rclcpp::get_logger("logger");

  // Pass parameters and initialize node
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<StaticFramePublisher>());
  rclcpp::shutdown();
  return 0;
}