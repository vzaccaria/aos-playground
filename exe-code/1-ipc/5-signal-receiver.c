#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
 
void my_handler(int sig_num) {
    printf("Received %d\n", sig_num);
}

int main() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    
    sa.sa_handler = &my_handler;
    sigaction(SIGUSR1, &sa, NULL);

    while(1) { sleep(1); }
    return 0;
}
