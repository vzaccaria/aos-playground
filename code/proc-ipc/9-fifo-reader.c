#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main () {
   
    const char * myfifo = "/tmp/myfifo";
    int fd = open(myfifo, O_RDONLY);
    if (fd <= 0) {
        printf("Cannot open fifo\n");
        return 1;
    }

    float data;
    int nb = read(fd, &data, sizeof(data));
    if (nb == 0) {
        printf("Read error\n");
        return 1;
    }
    
    printf("Read: %f\n", data);

    close(fd);
    return 0;
}
