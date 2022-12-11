#ifndef FTP_CONNECT_H
#define FTP_CONNECT_H

#include <libssh2.h>
#include <libssh2_sftp.h>
 
#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
 
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>

int SSH_Initial(void);
int SSH_Connect(const char* ip, int port);
int SSH_Login(const char* username, const char* pwd);
int SSH_FTP_Recieved(const char* filename, char* content, int *size);
int SSH_FTP_Send(const char* filename, char* content, size_t size);
void SSH_Disconnect(void);

#endif