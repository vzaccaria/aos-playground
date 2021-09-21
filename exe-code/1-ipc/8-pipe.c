#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

    float number;
    printf("Please insert a number: ");
    scanf("%f", &number);
    
    int fd[2];
    pipe(fd);
    
    int id = fork();
    if (id == 0) {
        // Child
        close(fd[0]);
        float result = sqrt(number);
        write(fd[1], &result, sizeof(result));
        close(fd[1]);        
        
    } else {
        // Parent
        close(fd[1]);
        float result = sqrt(number);
        float result_from_child;
        read(fd[0], &result_from_child, sizeof(result_from_child));
        close(fd[0]);
        wait(NULL);
        
        if (result_from_child == result) {
            printf("sqrt(%f)=%f\n", number, result);
        } else {
            printf("Incorrect computation? %f != %f\n", result_from_child, result);        
        }
        
    }

    return 0;
}
