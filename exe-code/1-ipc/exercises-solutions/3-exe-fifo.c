#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main() {

    int i, ret;
    char fifo_name[16];


    for (i=0; i<100; i++) {
        snprintf(fifo_name, 16, "/tmp/myfifo.%d", i);
        ret = mkfifo(fifo_name, 0660);
        if (ret == 0) {
            // FIFO created successfully
            break;
        }
    }
    
    if (ret != 0) {
        printf("Cannot create any FIFO\n");
        return 1;
    }
    
    int fd = open(fifo_name, O_RDONLY);
    if (fd < 0) {
        printf("Cannot open the FIFO\n");
        return 1;    
    }
    
    char incoming_str[200];
    for (i=0; i<200; i++) {
        read(fd, &incoming_str[i], sizeof(char));
        if (incoming_str[i] == '\n') {
            break;
        }
    }

    i = i - 1;

    for (int j=0; j<i/2; j++) {
        char tmp = incoming_str[j];
        incoming_str[j] = incoming_str[i-j];
        incoming_str[i-j] = tmp;
    }

    printf("Printed: %s\n", incoming_str);

    return 0;
}
