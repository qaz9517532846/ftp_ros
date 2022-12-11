#include <ftp_ros/ftp_status.h>
#include <stddef.h>

#define INTEM_NUM       (12)

static FTP_STATUS status;
static FTP_EVENT currentEvent;

typedef struct
{
    FTP_STATUS currentStatus;
    FTP_EVENT event;
    FTP_STATUS nextStatus;
    void (*action) (void);
} FTP_TABLE;

static FTP_TABLE ftpTable[INTEM_NUM] = {
    {COMM_INITIAL,          COMM_INITIAL_TO_SUCCESS,        COMM_SUCCESS,               NULL},
    {COMM_INITIAL,          COMM_INITIAL_TO_FAILED,         COMM_FAILED,                NULL},
    {COMM_SUCCESS,          LOGIN_TO_INITIAL,               LOGIN_INITIAL,              NULL},
    {LOGIN_INITIAL,         LOGIN_TO_SUCCESS,               LOGIN_SUCCESS,              NULL},
    {LOGIN_INITIAL,         LOGIN_TO_FAILED,                LOGIN_FAILED,               NULL},
    {LOGIN_SUCCESS,         SEND_FILE_TO_SUCCESS,           SEND_SUCCESS,               NULL},
    {LOGIN_SUCCESS,         SEND_FILE_TO_FAILED,            SEND_FAILED,                NULL},
    {LOGIN_SUCCESS,         REV_FILE_TO_SUCCESS,            REV_SUCCESS,                NULL},
    {LOGIN_SUCCESS,         REV_FILE_TO_FAILED,             REV_FAILD,                  NULL},
    {COMM_FAILED,           COMM_TO_RETRY,                  COMM_INITIAL,               NULL},
    {LOGIN_FAILED,          LOGIN_TO_RETRY,                 LOGIN_INITIAL,              NULL}};

FTP_STATUS Ftp_GetStaus(void)
{
    return status;
}

FTP_EVENT FTP_GetEvent(void)
{
    return currentEvent;
}

void FTP_SetEvent(FTP_EVENT event)
{
    currentEvent = event;
    for(int i = 0; i < INTEM_NUM; i++)
    {
        if(status == ftpTable[i].currentStatus && currentEvent == ftpTable[i].event)
        {
            status = ftpTable[i].nextStatus;
            if(ftpTable[i].action != NULL) ftpTable[i].action();
            break;
        }
    }
}