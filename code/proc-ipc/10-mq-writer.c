#include <errno.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#define MSG_SIZE    100
#define MSGQUEUE_NAME  "/bazinga"
#define NR_MESSAGES 3

char messages[3][MSG_SIZE];

unsigned int priorities[] = { 3, 0, 1 };

int main()
{
    strcpy(messages[0], "Scissors Cuts Paper.");
    strcpy(messages[1], "Rock Crushes Lizard.");
    strcpy(messages[2], "Paper Covers Rock.");

    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    // maxvalue: cat /proc/sys/fs/mqueue/queues_max
    attr.mq_msgsize = MSG_SIZE; // maxvalue: cat /proc/sys/fs/mqueue/msgsize_max
    attr.mq_flags = 0;
    attr.mq_curmsgs = 0;
    
    mqd_t mqd = mq_open(MSGQUEUE_NAME, O_CREAT | O_WRONLY, 0664, &attr);
    if (mqd < 0) {
        printf("Error - open\n");
        return 1;
    }
    for (int i = 0; i < NR_MESSAGES; ++i) {
        printf("%s\n", messages[i]);
        ssize_t ret = mq_send(mqd, messages[i], MSG_SIZE, priorities[i]);
        if (ret < 0) {
            printf("Error - send\n");
            return 1;
        }
        printf("Sent a message with priority=%d\n", priorities[i]);
    }
    mq_close(mqd);
    return 0;
}
