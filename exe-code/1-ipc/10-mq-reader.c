#include <errno.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#define MSG_SIZE 100
#define MSGQUEUE_NAME "/bazinga"


int main()
{
    char buf[MSG_SIZE];
    unsigned int prio;
    mqd_t mqd = mq_open(MSGQUEUE_NAME, O_RDONLY, 0664, NULL);
    if (mqd < 0) {
        printf ("error - mq_open()");
        return 1;
    }
    struct mq_attr attr;
    mq_getattr(mqd, &attr);

    printf ("Nr. messages in the queue: %ld / %ld [maxsize=%ld]\n",
        attr.mq_curmsgs,
        attr.mq_maxmsg,
        attr.mq_msgsize);

    while (attr.mq_curmsgs != 0) {
        ssize_t size = mq_receive(mqd, buf, MSG_SIZE, &prio);
        if (size < 0) {
            printf("main - mq_receive()");
            return 1;
        }
        printf("Received a message len=%ld with priority=%d: %s\n",
            size, prio, buf);

        mq_getattr (mqd, &attr);
    }
    printf("No messages left in the queue.\n");
    mq_close(mqd);
    return 0;    
    
}
