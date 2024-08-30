#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <semaphore.h>

int main(int argc, char *argv[])
{
    int mode = atoi(argv[1]);
    sem_t *sem = sem_open("/test", O_RDWR | O_CREAT, 0644, 0);
    if (SEM_FAILED == sem)
    {
        perror("sem_open erro");
        return -1;
    }

    while (1)
    {
        if (0 == mode) //申请资源
        {
            sem_wait(sem);
            printf("消费....\n");
        }
        else
        {
            sleep(1);
            sem_post(sem);
            printf("生产...\n");
        }
    }
    sem_close(sem);
    sem_unlink("/tmp");
    return 0;
}