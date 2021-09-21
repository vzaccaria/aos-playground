#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        printf("Ehi! Give me a pid!\n");
        return 1;
    }
    
    pid_t target_pid = atoi(argv[1]);
    
    kill(target_pid, SIGUSR1);
    
    return 0;
}
