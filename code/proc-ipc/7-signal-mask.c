#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


void my_handler(int) {
    printf("Handling the signal...\n");
}

int main() {

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    
    sa.sa_handler = &my_handler;
    sigaction(SIGINT, &sa, NULL);

    sigset_t my_sig_set;                        //2nd part
    sigemptyset(&my_sig_set);                   //2nd part
    sigaddset(&my_sig_set, SIGINT);             //2nd part
    sigprocmask(SIG_BLOCK, &my_sig_set, NULL);  //2nd part

    for(int i=0; i<5; i++) {
        printf("Sleeping...\n");
        sleep(1);
    }

    sigprocmask(SIG_UNBLOCK, &my_sig_set, NULL);    //2nd part

	return 0;
}
