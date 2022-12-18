#include <ftp_ros/ftp_config.h>
#include <ftp_ros/ftp_connect.h>
#include <ftp_ros/ftp_status.h>

static LIBSSH2_SESSION *session;
static LIBSSH2_SFTP *sftp_session;
static LIBSSH2_SFTP_HANDLE *sftp_handle;
static int sock;

static int WaitSocket(int socket_fd, LIBSSH2_SESSION *session)
{
    struct timeval timeout;
    int rc;
    fd_set fd;
    fd_set *writefd = NULL;
    fd_set *readfd = NULL;
    int dir;
 
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
 
    FD_ZERO(&fd);
 
    FD_SET(socket_fd, &fd);
 
    /* now make sure we wait in the correct direction */ 
    dir = libssh2_session_block_directions(session);

 
    if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
        readfd = &fd;
 
    if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
        writefd = &fd;
 
    rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);
 
    return rc;
}

static int SSH_FTP_Initial(void)
{
    int rc = 1;
    while(!sftp_session)
    {
        sftp_session = libssh2_sftp_init(session);
        if(!sftp_session)
        {
            if(libssh2_session_last_errno(session) == LIBSSH2_ERROR_EAGAIN)
            {
                printf("non-blocking init\n");
                rc = WaitSocket(sock, session); /* now we wait */ 
            }
            else
            {
                printf("Unable to init SFTP session\n");
                rc = -1;
            }
        }
    }

    return rc;
}

int SSH_Initial(void)
{
    int rc = 0;
    rc = libssh2_init(0);

    if(rc != 0)
        printf("libssh2 initialization failed (%d)\n", rc);

    return rc;
}

int SSH_Connect(const char* ip, int port)
{
    int rc = 0;
    struct sockaddr_in sin;
    sock = socket(AF_INET, SOCK_STREAM, 0);
 
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = inet_addr(ip);
    if(connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0)
    {
        printf("failed to connect!\n");
        FTP_SetEvent(COMM_INITIAL_TO_FAILED);
        rc = -1;
    }
    else
    {
        session = libssh2_session_init();
        if(!session) rc = -1;
        rc = libssh2_session_handshake(session, sock);
        FTP_SetEvent(COMM_INITIAL_TO_SUCCESS);
    }

    return rc;
}

int SSH_Login(const char* username, const char* pwd)
{
    FTP_SetEvent(LOGIN_TO_INITIAL);
    const char *fingerprint;
    int rc = 0;

    if(rc)
    {
        FTP_SetEvent(LOGIN_TO_FAILED);
        printf("Failure establishing SSH session: %d\n", rc);
    }
    else
    {
        libssh2_session_set_blocking(session, 0);
        fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

        printf("Fingerprint: ");
        for(int i = 0; i < 20; i++)
            printf("%02X ", (unsigned char)fingerprint[i]);

        fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

        printf("Fingerprint: ");
        for(int i = 0; i < 20; i++)
            printf("%02X ", (unsigned char)fingerprint[i]);

        while((rc = libssh2_userauth_password(session, username, pwd)) == LIBSSH2_ERROR_EAGAIN);
        if(rc)
        {
            printf("Authentication by password failed.\n");
            FTP_SetEvent(LOGIN_TO_FAILED);
        }
        else
            FTP_SetEvent(LOGIN_TO_SUCCESS);
    }

    return rc;
}

int SSH_FTP_Recieved(const char* filename, char* content, int *size)
{
    int rc = 0;
    int timeoutCNT = 0;
    struct timeval timeout;
    fd_set fd;
    fd_set fd2;

    rc = SSH_FTP_Initial();
    if(rc >= 0)
    {
        while(!sftp_handle && timeoutCNT < 5)
        {
            sftp_handle = libssh2_sftp_open(sftp_session, filename, LIBSSH2_FXF_READ, 0);
 
            if(!sftp_handle)
            {
                if(libssh2_session_last_errno(session) != LIBSSH2_ERROR_EAGAIN)
                {
                    rc = -1;
                    printf("Unable to open file with SFTP\n");
                }
                    
            }
            else
            {
                printf("non-blocking open\n");
                rc = WaitSocket(sock, session); /* now we wait */ 
            }

            sleep(1);
            timeoutCNT++;
        }
    }

    while(sftp_handle)
    {

        rc = libssh2_sftp_read(sftp_handle, content, sizeof(content));

        *size = sizeof(content);
 
        if(rc != LIBSSH2_ERROR_EAGAIN)
        {
            break;
        }
 
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
 
        FD_ZERO(&fd);
        FD_ZERO(&fd2);
        FD_SET(sock, &fd);
        FD_SET(sock, &fd2);
 
        /* wait for readable or writeable */ 
        rc = select(sock + 1, &fd, &fd2, NULL, &timeout);
        if(rc <= 0)
        {
            printf("SFTP download timed out: %d\n", rc);
            break;
        }

        sleep(1);
    };

    return rc;
}

int SSH_FTP_Send(const char* filename, char* content, size_t size)
{
    int rc = 0;
    struct timeval timeout;
    int timeoutCNT = 0;
    fd_set fd;
    fd_set fd2;

    rc = SSH_FTP_Initial();
    if(rc >= 0)
    {
        while(!sftp_handle && timeoutCNT < 5)
        {
            sftp_handle = libssh2_sftp_open(sftp_session, filename, LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT,
                                                        LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
                                                        LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);
            timeoutCNT++;
            sleep(1);
        }
        
        if(sftp_handle)
        {
            while(1)
            {
                while(rc != size) rc = libssh2_sftp_write(sftp_handle, content, size);
 
                if(rc != LIBSSH2_ERROR_EAGAIN)
                {
                    /* error or end of file */ 
                    break;
                }
 
                timeout.tv_sec = 1;
                timeout.tv_usec = 0;
 
                FD_ZERO(&fd);
                FD_ZERO(&fd2);
                FD_SET(sock, &fd);
                FD_SET(sock, &fd2);
 
                rc = select(sock + 1, &fd, &fd2, NULL, &timeout);
                if(rc <= 0)
                {
                    printf("SFTP upload timed out: %d\n", rc);
                    break;
                }

                sleep(1);
            }
            printf("SFTP upload done!\n");
        }
        else
        {
            printf("SFTP failed to open destination path: %s\n", filename);
        }
    }
    libssh2_sftp_close(sftp_handle);
}

void SSH_Disconnect(void)
{
    libssh2_sftp_shutdown(sftp_session);
    libssh2_session_disconnect(session, "Normal Shutdown");
    libssh2_session_free(session);
    close(sock);
    libssh2_exit();
}