#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct person {
	char name[100];
	char surname[100];
	unsigned int year_of_birth;
};

int main() {

    int fd = shm_open("/myshm", O_RDWR, 0666);
    if (fd < 0) {
        printf("SHM Error: %s\n", strerror(errno));
        return 1;
    }

    struct stat stat;
    fstat(fd, &stat);
    printf("Shared memory size: %d bytes\n", stat.st_size);
    
    
    void *ptr_shm = mmap(NULL, sizeof(struct person), PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr_shm == MAP_FAILED) {
        printf("MAP FAILED: %s\n", strerror(errno));
        return 1;
    }


    struct person * p = (struct person *)ptr_shm;


    sem_t *my_sem = sem_open("/myshm_sem", 0);

    printf("Please insert the name: ");
    scanf("%s", p->name);
    printf("Please insert the surname: ");
    scanf("%s", p->surname);
    printf("Please insert the year of birth: ");
    scanf("%d", &p->year_of_birth);
    sem_post(my_sem);
    
    munmap(ptr_shm, sizeof(struct person));
    close(fd);
    
    return 0;
}
