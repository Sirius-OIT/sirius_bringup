import os

from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory

argument = 'laser 0 0 1 0 0 0'

def generate_launch_description():
    
    kobuki_node_pkg = get_package_share_directory('kobuki_node')
    kobuki_node_path = os.path.join(kobuki_node_pkg, 'launch')

    tf_broadcaster_pkg = get_package_share_directory('static_tf2_broadcaster')
    tf_broadcaster_path = os.path.join(tf_broadcaster_pkg, 'src')

    urg_node_pkg = get_package_share_directory('urg_node')
    urg_node_path = os.path.join(urg_node_pkg, 'launch')
    
    turtlebot3_cartographer_pkg = get_package_share_directory('turtlebot3_cartographer')
    turtlebot3_cartographer_path = os.path.join(turtlebot3_cartographer_pkg, 'launch')

    return LaunchDescription([
        # IncludeLaunchDescription(
        #     PythonLaunchDescriptionSource(
        #         [kobuki_node_path, '/kobuki_node-launch.py']
        #     )
        # ),

        Node(
            package = tf_broadcaster_pkg,
            namespace = 'sirius_tf2_broadcaster',
            executable = 'static_tf2_broadcaster',
            arguments=['--ros-args', 'laser 0 0 1 0 0 0']
        ),

        # IncludeLaunchDescription(
        #     PythonLaunchDescriptionSource(
        #         [urg_node_path, '/urg_node_launch.py']
        #     )
        # )

        # IncludeLaunchDescription(
        #     PythonLaunchDescriptionSource(
        #         [turtlebot3_cartographer_path, '/cartographer.launch.py']
        #     )
        # ),

    ])