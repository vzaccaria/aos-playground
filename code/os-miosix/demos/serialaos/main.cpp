
// Test code for serialaos driver

#include <cstdio>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd=open("/dev/serialaos",O_RDWR);
    if(fd<0) perror("open");
    else {
        printf("Open ok\n");
        for(;;)
        {
            char buffer[16];
            int count=read(fd,buffer,sizeof(buffer));
            if(count<0) perror("read");
            else {
                printf("read %d bytes\n",count);
                int written=write(fd,buffer,count);
                if(written<0) perror("write");
            }
        }
    }
}
