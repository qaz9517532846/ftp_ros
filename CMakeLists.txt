cmake_minimum_required(VERSION 3.0.2)
project(ftp_ros)

# add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  std_msgs
)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
add_service_files(
  FILES
  ftpFile.srv
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
   INCLUDE_DIRS include
   LIBRARIES ftp_ros
#   CATKIN_DEPENDS message_generation roscpp std_msgs
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(ftp_ros src/ftp_ros.cpp
                       src/ftp_file.cpp
                       src/ftp_connect.cpp
                       src/ftp_status.cpp)
target_link_libraries(ftp_ros ${catkin_LIBRARIES} /usr/lib/x86_64-linux-gnu/libssh2.so)
add_dependencies(ftp_ros ftp_ros_generate_messages_cpp)


add_executable(ftp_client src/ftp_client.cpp)
target_link_libraries(ftp_client ${catkin_LIBRARIES})
add_dependencies(ftp_client ftp_ros_generate_messages_cpp)
