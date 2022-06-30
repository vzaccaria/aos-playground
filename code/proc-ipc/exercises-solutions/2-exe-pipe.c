#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

float compute_sum(int start, int end) {

    float sum = 0;

    for(int i = start; i <= end; i++) {
        sum += pow(1.5, i);
    }
    
    return sum;

}

int main() {

    int outer_pipe[2];
    int inner_pipe[2];
    
    pipe(outer_pipe);

    int id = fork();
    
    if (id == 0) {
        close(outer_pipe[0]);
        pipe(inner_pipe);

        id = fork();
        if (id == 0) {
            // Grandchild
            close(inner_pipe[0]);
            close(outer_pipe[1]);
            float res = compute_sum(9, 12);
            write(inner_pipe[1], &res, sizeof(res)); 
            close(inner_pipe[1]);
        } else {
            // Child
            float res = compute_sum(5, 8);
            close(inner_pipe[1]);
            float from_child;
            read(inner_pipe[0], &from_child, sizeof(from_child));
            wait(NULL);
            res = res + from_child;
            write(outer_pipe[1], &res, sizeof(res)); 
            close(outer_pipe[1]);
        }
    } else {
        close(outer_pipe[1]);
        // Parent
        float res = compute_sum(1, 4);
        float from_children;
        read(outer_pipe[0], &from_children, sizeof(from_children)); 
        wait(NULL);
        res = res + from_children;
        printf("total_sum=%f\n", res);
        close(outer_pipe[0]);
    }

    return 0;
}
