#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <unistd.h>

struct fibo_status {
    char done;
	int a;
	int b;
};

int main() {

    int fd = shm_open("/fibo_shm", O_RDWR, 0666);
    if (fd < 0) {
        printf("SHM Error: %s", strerror(errno));
        return 1;
    }

    void *ptr_shm = mmap(NULL, sizeof(struct fibo_status), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr_shm == MAP_FAILED) {
        printf("MAP FAILED: %s\n", strerror(errno));
        return 1;
    }

    struct fibo_status * p = (struct fibo_status *)ptr_shm;

    sem_t *my_sem_1 = sem_open("/fibo_sem_1", 0);
    sem_t *my_sem_2 = sem_open("/fibo_sem_2", 0);

    int c=0;
    while(p->done == 0) {
        sem_wait(my_sem_2);
        c = p->a + p->b;
        p->a = p->b;
        p->b = c;
        sem_post(my_sem_1);
    }
    
    munmap(ptr_shm, sizeof(struct fibo_status));
    shm_unlink("/fibo_shm");
    sem_close(my_sem_1);
    sem_close(my_sem_2);
    sem_unlink("/fibo_sem_1");
    sem_unlink("/fibo_sem_2");
    
    return 0;
}
