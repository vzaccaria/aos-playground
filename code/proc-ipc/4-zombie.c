#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {

    int i, ret;
    
    for (i=0; i<20; i++) {
        ret = fork();
        if (ret == 0) break;
    }

    if (ret == 0) {
        // Child
        printf("I'm the child %d!\n", getpid());
        return 123;
    } else {
        // Parent
        printf("I'm the parent and I sleep!\n");
        while(1) {
            int pid = wait(&ret);    // 2nd part
            if (pid < 0) {           // 2nd part
                return 0;            // 2nd part
            }
            printf("Child %d returned %d.\n", pid, WEXITSTATUS(ret));    // 2nd part
            sleep(1);
        }
    }

    return 0;
}
