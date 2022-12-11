#include <ftp_ros/ftp_file.h>
#include <ftp_ros/ftp_status.h>

int FileRead(std::string filename, char* fileContent)
{
    FILE *fp;
    struct stat st;
    uint64_t size = 0;
    int read_len;
    int content_pos = 0;

    if((fp = fopen(filename.c_str(), "rb")) != NULL)
    {
        stat(filename.c_str(), &st);
        size = st.st_size;

        fseek(fp, SEEK_SET, 0);
        read_len = fread(fileContent, 1, size, fp);
        fclose(fp);
    }

    return size;
}

bool FileWrite(std::string filename, char* fileContent, int size)
{
    FILE *fp;
    uint64_t total_len, count, write_len;
    bool result;

    total_len = size;
    if((fp = fopen(filename.c_str(), "wb")) != NULL)
    {
        fseek(fp, 0, SEEK_END);
        write_len = fwrite(fileContent, 1, size, fp);
        total_len -= write_len;
        fclose(fp);
        result = total_len == 0 ? true : false;
    }
    else
    {
        result = false;
    }

    return result;
}
