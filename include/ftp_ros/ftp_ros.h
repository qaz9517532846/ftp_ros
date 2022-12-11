#ifndef FTP_ROS_H
#define FTP_ROS_H

#include <ros/ros.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <string.h>

#include <ftp_ros/ftp_connect.h>
#include <ftp_ros/ftp_file.h>
#include <ftp_ros/ftp_status.h>
#include <ftp_ros/ftpFile.h>

class ftpROS
{
    public:
        ftpROS(std::string name);
        ~ftpROS();

        void ftp_spin(void);

    private:
        void ftp_connect(std::string ip, int port);
        void ftp_disconnect(void);
        bool ftp_file(ftp_ros::ftpFile::Request &req, ftp_ros::ftpFile::Response &res);

        ros::NodeHandle private_nh_;
        ros::ServiceServer ftpServer;

        std::string ipAddress;
        int port;

        std::string username;
        std::string password;
};

#endif