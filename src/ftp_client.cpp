#include <ros/ros.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string>
#include <ftp_ros/ftpFile.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "ftp_client");

    ros::NodeHandle n;

    ros::ServiceClient ftp_client = n.serviceClient<ftp_ros::ftpFile>("/ftp_ros/ftp_file");
    ftp_ros::ftpFile ftp_srv;
    ftp_srv.request.mode = "received";
    ftp_srv.request.srcPath = "/home/zmtech/catkin_ws/123.txt";
    ftp_srv.request.desPath = "/home/zmtech/catkin_ws/456.txt";
    if (ftp_client.call(ftp_srv))
    {
        ROS_INFO("response: %ld", (long int)ftp_srv.response.result);
    }
    else
    {
        ROS_ERROR("Failed to call service ftp_ros");
        return 1;
    }

    return 0;
}