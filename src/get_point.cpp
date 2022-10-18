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

    private:
        void callback(const nav_msgs::msg::Odometry::SharedPtr data)
        {
            boost::circular_buffer<int> buf(1);
            if(flg == true){
                buf.push_front(2);
                flg = false;
            }
            std::cout << data->pose.pose.position.x << std::endl;
            // distance_ = get_disctance(data, buf[0]);
            // if(distance_ > 5.0){
            //     flg = true;
            //     yaml_file_ << "  ";
            //     yaml_file_ << "- [ ";
            //     yaml_file_ << point_.x << ", ";
            //     yaml_file_ << point_.y << ", ";
            //     yaml_file_ << point_.z << ", ";
            //     yaml_file_ << qua_.x << ", ";
            //     yaml_file_ << qua_.y << ", ";
            //     yaml_file_ << qua_.z << ", ";
            //     yaml_file_ << qua_.w;
            //     yaml_file_ << " ]";
            //     yaml_file_ << std::endl;
            // }
        }

        double get_disctance(const nav_msgs::msg::Odometry::SharedPtr data1, const nav_msgs::msg::Odometry::SharedPtr data2)
        {
            double x1_ = data1->pose.pose.position.x;
            double x2_ = data2->pose.pose.position.x;
            double y1_ = data1->pose.pose.position.y;
            double y2_ = data2->pose.pose.position.y;

            return std::hypot((x1_ - x2_), (y1_ - y2_));
        }

        double x1_;
        double x2_;
        double y1_;
        double y2_;
        
        int count_;
        double distance_;
        std::string yaml_file_path_;
        std::ofstream yaml_file_;

        bool flg=true;
        // boost::circular_buffer<nav_msgs::msg::Odometry::SharedPtr> buf(1);   
        rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GetPoint>());
    rclcpp::shutdown();
    return 0;
}