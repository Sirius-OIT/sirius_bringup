cmake_minimum_required(VERSION 3.5)
project(sirius_bringup)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake_auto REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(Boost REQUIRED system COMPONENTS)
find_package(yaml-cpp REQUIRED)

ament_auto_find_build_dependencies()

ament_auto_add_executable(static_tf2_broadcaster src/static_tf2_broadcaster.cpp)
ament_auto_add_executable(get_point src/get_point.cpp)

ament_target_dependencies(get_point yaml-cpp)

install(DIRECTORY launch
  DESTINATION share/sirius_bringup
)

install(DIRECTORY config
  DESTINATION share/sirius_bringup
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_auto_package()
