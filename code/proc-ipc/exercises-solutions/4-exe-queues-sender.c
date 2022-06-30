#include <mqueue.h>
#include <stdio.h>
#include <string.h>

#define PRIORITY_HIGH 2
#define PRIORITY_MEDIUM 1
#define PRIORITY_LOW 0

#define MAX_LOG_MSG_LENGTH 200

int main() {

    char buffer[MAX_LOG_MSG_LENGTH];
 
    mqd_t mq = mq_open("/logqueue", O_WRONLY);
    
    strcpy(buffer, "Useless message");
    mq_send(mq, buffer, MAX_LOG_MSG_LENGTH, PRIORITY_LOW);
    strcpy(buffer, "Not so important message");
    mq_send(mq, buffer, MAX_LOG_MSG_LENGTH, PRIORITY_MEDIUM);
    strcpy(buffer, "Super important message");
    mq_send(mq, buffer, MAX_LOG_MSG_LENGTH, PRIORITY_HIGH);

    mq_close(mq);

    return 0;
}
