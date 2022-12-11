#ifndef FTP_FILE_H
#define FTP_FILE_H

#ifdef __cplusplus

#include <string>
#include <stdio.h>
#include <sys/stat.h>

#include <time.h>

extern "C" {
#endif

typedef enum
{
    READ,
    WRITE,
    STAND_BY,
} FILE_HANDLE;

#ifdef __cplusplus

int FileRead(std::string filename, char* fileContent);
bool FileWrite(std::string filename, char* fileContent, int size);

}

#endif

#endif