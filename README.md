# ftp_ros

FTP file send and recieve system using linssh2 library.

------

## Built with

- ROS Melodic Morenia under Ubuntu 18.04 LTS

- ROS Noetic Ninjemys under Ubuntu 20.04 LTS

------

## Getting Started

### Installation

- Installation ros package.

    ``` $ sudo apt-get install libssh-dev ssh```

- clone ftp_ros package.

``` bash
$ git clone https://github.com/qaz9517532846/ftp_ros.git
```

### Run

------

- The ftp ros service.

``` bash
$ roslaunch ftp_ros ftp_ros.launch
```

- The ftp ros client.

``` bash
$ rosrun ftp_ros ftp_client
```

------

## Reference:

[1]. libssh, https://www.libssh.org/

------

## License:

This repository is for your reference only. copying, patent application, academic journals are strictly prohibited.

Copyright © 2022 ZM Robotics Software Laboratory.
