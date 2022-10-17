#include <fstream>
#include <string>
#include <vector>
#include <boost/circular_buffer.hpp>

#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/point.hpp"

class GetPoint : public rclcpp::Node
{
    public:
        explicit GetPoint(), count_(0), distance_(0.0)
        {
            current_pose_ = this->create_subscription<nav_msgs::msg::Odometry>("/odom", 10, std::bind(&GetGoal::callback, this, _1));
            this->declare_parameter("yaml_file_path_");
            this->get_parameter("yaml_file_path_", yaml_file_path_);

            yaml_file_.open(yaml_file_path_, std::ios::out);
            std::string text1 = "---";
            std::string text2 = "points: ";
            yaml_file_ << text1 << std::endl;
            yaml_file_ << text2 << "\n";

        }

        ~GetPoint()
        {
            yaml_file_.close();
        }


    private:
        void callback(const nav_msgs::msg::Odometry::SharedPtr data) const
        {
            point_ = data->pose.pose.position;
            gua_ = data->pose.pose.orientation;
            if(flg == true){
                c_buf.push_front(point_);
                flg = false;
            }
            distance_ = get_disctance(point_, c_buf[0]);
            if(distance_ > 5.0){
                flg = true;
                yaml_file_ << "  ";
                yaml_file_ << "- [ ";
                yaml_file_ << point_.x << ", ";
                yaml_file_ << point_.y << ", ";
                yaml_file_ << point_.z << ", ";
                yaml_file_ << qua_.x << ", ";
                yaml_file_ << qua_.y << ", ";
                yaml_file_ << qua_.z << ", ";
                yaml_file_ << qua_.w;
                yaml_file_ << " ]";
                yaml_file_ << "\n";
            }
        }

        double get_disctance(geometry_msgs::msg::Point point1, geometry_msgs::msg::Point point2)
        {
            return std::hypot((point2.x - point1.x), (point2.y - point1.y));
        }

        int count_;
        double distance_;
        std::string yaml_file_path_;
        bool flg=true;
        boost::circular_buffer<geometry_msgs::msg::Point> c_buf(1);
        std::ofstream yaml_file_;

        geometry_msgs::msg::Point point_;
        geometry_msgs::msg::Quaternion qua_;
}