#include <signal.h>
#include <stdio.h>

int main() {

    sigset_t waitset;
    sigemptyset(&waitset);
    sigaddset(&waitset, SIGINT);
    sigprocmask(SIG_BLOCK, &waitset, NULL);
    
    siginfo_t info;
    
    struct timespec timeout;
    timeout.tv_sec  = 10;
    timeout.tv_nsec = 0;
    
    int result = sigtimedwait(&waitset, &info, &timeout);

    if (result < 0) {
        printf("\nTimeout.\n", result);
    } else {
        printf("\nSignal received.\n", result);    
    }

	return 0;
}
