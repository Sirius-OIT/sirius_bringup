from launch import LaunchDescription
from launch_ros.actions import Node
from launch_ros import IncludeLaunchDescription

def generate_launch_description():
    return LaunchDescription(
        launch_ros.IncludeLaunchDescription(
            package='kobuki_node',
            launch='kobuki_node-launch.py'
        ),
        Node(
            package='kobuki_keyop',
            executable='kobuki_keyop_node',
        )
    )