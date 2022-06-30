#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <unistd.h>

struct person {
	char name[100];
	char surname[100];
	unsigned int year_of_birth;
};

int main() {

    int fd = shm_open("/myshm", O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        printf("SHM Error: %s", strerror(errno));
        return 1;
    }
    
    ftruncate(fd, sizeof(struct person));

    void *ptr_shm = mmap(NULL, sizeof(struct person), PROT_READ, MAP_SHARED, fd, 0);
    if (ptr_shm == MAP_FAILED) {
        printf("MAP FAILED: %s\n", strerror(errno));
        return 1;
    }

    struct person * p = (struct person *)ptr_shm;

    sem_t *my_sem = sem_open("/myshm_sem", O_CREAT, 0666, 0);

    while(1) {
        sem_wait(my_sem);
        printf("%s %s %d\n", p->name, p->surname, p->year_of_birth);
    }
    
    munmap(ptr_shm, sizeof(struct person));
    shm_unlink("/myshm");
    
    return 0;
}
