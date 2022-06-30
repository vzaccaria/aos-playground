#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int sum = 0;


void my_handler(int signum) {
    printf("Sum: %d\n", sum);
    exit(0);
}

int main() {

    struct sigaction act_usr1;
    memset(&act_usr1, sizeof(struct sigaction), 0);
    act_usr1.sa_handler = my_handler;

    struct sigaction act_term;
    memset(&act_term, sizeof(struct sigaction), 0);
    act_term.sa_handler = SIG_IGN;


    sigaction(SIGUSR1, &act_usr1, NULL);
    sigaction(SIGTERM, &act_term, NULL);

    int i;
    for (i=0; i<=100; i++) {
        printf("%d\n", i);
        sum += i;
        sleep(1);
    }

    printf("Sum: %d\n", sum);

    return 0;
}
