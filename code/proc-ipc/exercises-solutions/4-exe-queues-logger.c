#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define PRIORITY_HIGH 2
#define PRIORITY_MEDIUM 1
#define PRIORITY_LOW 0

#define FILENAME "log.txt"

#define MAX_LOG_MSG_LENGTH 200


const char *priorities_text[] = {
    "LOW",
    "MEDIUM",
    "HIGH"
};

void print_to_file(const struct tm * timeinfo, int priority, const char *text) {

    static FILE *log_file = NULL;

    if (log_file == NULL) {
        log_file = fopen(FILENAME, "a");
    }

    if (log_file != NULL) {
        const char *ptxt;
        ptxt = priority >= 0 && priority <= 2 ? priorities_text[priority] : "UNKW";
        fprintf(log_file, "%2d:%2d:%2d [%s] %s\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, ptxt, text);
        fflush(log_file);
    }
}

void main_loop(mqd_t mq) {

    time_t rawtime;
    struct tm * timeinfo;

    while(1) {

        char msg[MAX_LOG_MSG_LENGTH];
        int priority;
        int ret = mq_receive(mq, msg, MAX_LOG_MSG_LENGTH, &priority);
        
        if (ret > 0) {
            time ( &rawtime );
            timeinfo = localtime ( &rawtime );
            print_to_file(timeinfo, priority, msg);
        }
    
    }

}

int main() {

    struct mq_attr attr;
    
    attr.mq_flags   = 0;
    attr.mq_maxmsg  = 10;
    attr.mq_msgsize = MAX_LOG_MSG_LENGTH;
    attr.mq_curmsgs = 0;

    mqd_t mq = mq_open("/logqueue", O_RDONLY | O_CREAT, 0644, &attr);
    
    if (mq < 0) {
        printf("Cannot open the queue: %s\n", strerror(errno));
        return 1;
    }
    
    main_loop(mq);
    

    return 0;
}
