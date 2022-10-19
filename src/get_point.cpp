#include <fstream>
#include <string>
#include <vector>
#include <boost/circular_buffer.hpp>

#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "yaml-cpp/yaml.h"

class GetPoint : public rclcpp::Node
{
    public:
        GetPoint() : Node("get_point"), count_(0), distance_(0.0)
        {
            subscription_ = this->create_subscription<nav_msgs::msg::Odometry>("/odom", 10, std::bind(&GetPoint::callback, this, std::placeholders::_1));
            this->declare_parameter("yaml_file_path_");
            this->get_parameter("yaml_file_path_", yaml_file_path_);

            yaml_file_.open(yaml_file_path_, std::ios::out);
            std::string text1 = "---";
            std::string text2 = "points: ";
            yaml_file_ << text1 << std::endl;
            yaml_file_ << text2 << std::endl;

        }

        ~GetPoint()
        {
            yaml_file_.close();
        }

        struct Geometry_msgs
        {
            double x_;
            double y_;
            double z_;
            double x_qua_;
            double y_qua_;
            double z_qua_;
            double w_qua_;
        };

    private:
        void callback(const nav_msgs::msg::Odometry::SharedPtr data)
        {
            
            // boost::circular_buffer<geometry_msgs::msg::Pose> buf(1);
            boost::circular_buffer<Geometry_msgs> buf(1);
            if(flg == true){
                geometry_msgs.x_ = data->pose.pose.position.x;
                geometry_msgs.y_ = data->pose.pose.position.y;
                geometry_msgs.z_ = data->pose.pose.position.z;
                geometry_msgs.x_qua_ = data->pose.pose.orientation.x;
                geometry_msgs.y_qua_ = data->pose.pose.orientation.y;
                geometry_msgs.z_qua_ = data->pose.pose.orientation.z;
                geometry_msgs.w_qua_ = data->pose.pose.orientation.w;
                buf.push_front(geometry_msgs);
                flg = false;
            }
            distance_ = this->get_distance(geometry_msgs, buf[0]);
            if(distance_ > 5.0){
                flg = true;
                yaml_file_ << "  ";
                yaml_file_ << "- [ ";
                yaml_file_ << data->pose.pose.position.x << ", ";
                yaml_file_ << data->pose.pose.position.y << ", ";
                yaml_file_ << data->pose.pose.position.z << ", ";
                yaml_file_ << data->pose.pose.orientation.x << ", ";
                yaml_file_ << data->pose.pose.orientation.y << ", ";
                yaml_file_ << data->pose.pose.orientation.z << ", ";
                yaml_file_ << data->pose.pose.orientation.w;
                yaml_file_ << " ]";
                yaml_file_ << std::endl;
            }
        }

        double get_distance(Geometry_msgs data1, Geometry_msgs data2)
        {
            double x1_ = data1.x_;
            double x2_ = data2.x_;
            double y1_ = data1.y_;
            double y2_ = data2.y_;

            return std::hypot((x1_ - x2_), (y1_ - y2_));
        }
        
        int count_;
        double distance_;
        std::string yaml_file_path_;
        std::ofstream yaml_file_;

        bool flg=true;
        // boost::circular_buffer<geometry_msgs::msg::Pose> buf(1);   
        rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscription_;

        Geometry_msgs geometry_msgs;
        
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GetPoint>());
    rclcpp::shutdown();
    return 0;
}