#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
    int mode = atoi(argv[1]);
    // 1 打开共享内存文件
    int fd = shm_open("/test2", O_RDWR | O_CREAT, 0644);
    if (-1 == fd)
    {
        perror("shm_open error");
        return -1;
    }

    // 2 设置缓冲大小
    int ret = ftruncate(fd, 4096);
    if (-1 == ret)
    {
        perror("ftruncate error");
        return -1;
    }

    // 3 映射
    void *addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
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
            char rbuf[4096] = {0};
            memcpy(rbuf, addr, 4096);
            printf("data:%s\n", rbuf);
        }
        else
        {
            char sbuf[4696] = {0};
            sprintf(sbuf, "%d", i++);
            memcpy(addr, sbuf, strlen(sbuf));
            sleep(1);
        }
    }
    // 4 解除映射
    munmap(addr, 4096);

    // 5 删除共享内存
    shm_unlink("/test2");

    return 0;
}