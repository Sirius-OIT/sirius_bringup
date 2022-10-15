import os

from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    laser_frame_config_arg = DeclareLaunchArgument(
        'tf2_config', default_value='laser_frame_config.yaml'
    )

    config_pkg = get_package_share_directory('sirius_bringup')
    config_path = os.path.join(config_pkg, 'config', 'laser_frame_config.yaml')
    
    kobuki_node_pkg = get_package_share_directory('kobuki_node')
    kobuki_node_path = os.path.join(kobuki_node_pkg, 'launch')

    urg_node_pkg = get_package_share_directory('urg_node')
    urg_node_path = os.path.join(urg_node_pkg, 'launch')
    
    turtlebot3_cartographer_pkg = get_package_share_directory('turtlebot3_cartographer')
    turtlebot3_cartographer_path = os.path.join(turtlebot3_cartographer_pkg, 'launch')

    return LaunchDescription([
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                [kobuki_node_path, '/kobuki_node-launch.py']
            )
        ),

        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                [urg_node_path, '/urg_node_launch.py']
            )
        ),

        Node(
            package = 'sirius_bringup',
            namespace = 'sirius',
            executable = 'static_tf2_broadcaster',
            name = 'static_tf2_broadcaster',
            parameters = [config_path]
        ),

        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                [turtlebot3_cartographer_path, '/cartographer.launch.py']
            )
        ),

    ])