#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>

int main(int argc, char **argv)
{
    int mode = atoi(argv[1]);

// 信号量代码
#if 1
    sem_t *sem = sem_open("/test", O_RDWR | O_CREAT, 0644, 0);
    if (SEM_FAILED == sem)
    {
        perror("sem_open erro");
        return -1;
    }
#endif

    // 1 打开共享内存文件
    int fd = shm_open("/test2", O_RDWR | O_CREAT, 0644);
    if (-1 == fd)
    {
        perror("shm_open error");
        return -1;
    }

    // 2 设置缓冲大小
    int ret = ftruncate(fd, 2048);
    if (-1 == ret)
    {
        perror("ftruncate error");
        return -1;
    }

    // 3 映射
    void *addr = mmap(NULL, 2048, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("mmap error");
        return -1;
    }
    int i = 0;
    while (1)
    {
        if (0 == mode)
        {
            sem_wait(sem);
            char rbuf[2048] = {0};
            memcpy(rbuf, addr, 2048);
            printf("data:%s\n", rbuf);
            // if (rbuf[0]=='8')
            // {
            //     printf("读退出.....\n");
            //     break;
            // }
        }
        else
        {
            char sbuf[2048] = {0};
            sprintf(sbuf, "%d", i++);
            memcpy(addr, sbuf, strlen(sbuf));
            sem_post(sem);
            // if (i == 8)
            // {
            //     printf("写退出.....\n");
            //     break;
            // }

            sleep(1);
        }
    }
    // 4 解除映射
    munmap(addr, 2048);

    // 5 删除共享内存
    shm_unlink("/test2");

    // 释放信号量
    sem_close(sem);
    sem_unlink("/tmp");
    return 0;
}