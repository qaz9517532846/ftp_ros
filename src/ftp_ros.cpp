#include <ftp_ros/ftp_ros.h>

#define BUFFER_SIZE (1024 * 1024)

ftpROS::ftpROS(std::string name) : private_nh_("~")
{
    SSH_Initial();
    private_nh_.param<std::string>("ip_address", ipAddress, "127.0.0.1");
    private_nh_.param<std::string>("username", username, "zmtech");
    private_nh_.param<std::string>("passwd", password, "zmtech");
    private_nh_.param<int>("port", port, 22);

    ftpServer = private_nh_.advertiseService("ftp_file", &ftpROS::ftp_file, this);
}

ftpROS::~ftpROS()
{

}

void ftpROS::ftp_connect(std::string ip, int port)
{
    SSH_Connect(ip.c_str(), port);
}

void ftpROS::ftp_disconnect(void)
{
    SSH_Disconnect();
}

bool ftpROS::ftp_file(ftp_ros::ftpFile::Request &req, ftp_ros::ftpFile::Response &res)
{
    int rc = -1, size = 0;
    char content[BUFFER_SIZE];
    if(req.mode == "send")
    {
        size = FileRead(req.srcPath, content);
        ROS_INFO("Send Finish size = %d", size);
        if(size > 0)
            rc = SSH_FTP_Send(req.desPath.c_str(), content, size);
        else
            rc = -1;
    }
    else if(req.mode == "received")
    {
        rc = SSH_FTP_Recieved(req.desPath.c_str(), content, &size);
        ROS_INFO("Send Finish size = %d, content = %s", size, content);
        if(rc >= 0) FileWrite(req.srcPath, content, size);
    }

    res.result = rc >= 0 ? true : false;

    return true;
}

void ftpROS::ftp_spin(void)
{
    while(ros::ok())
    {
        ros::spinOnce();
        FTP_STATUS status = Ftp_GetStaus();
        switch(status)
        {
            case COMM_INITIAL:
                ftp_connect(ipAddress, port);
                break;
            case COMM_SUCCESS:
                SSH_Login(username.c_str(), password.c_str());
                break;
            case COMM_FAILED:
                ftp_disconnect();
                break;
            case LOGIN_INITIAL:
                break;
            case LOGIN_SUCCESS:
                break;
            case LOGIN_FAILED:
                break;
            default:
                break;
        }
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "ftp_ros");
    ftpROS ftpROS(ros::this_node::getName());
    ftpROS.ftp_spin();
    return 0;
}