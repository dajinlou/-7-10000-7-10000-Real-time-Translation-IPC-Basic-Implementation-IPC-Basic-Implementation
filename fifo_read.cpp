#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define P_FIFO "/tmp/p_fifo"

int main()
{
    char cache[100];
    int fd;

    memset(cache, 0, sizeof cache); // 初始化内存
    if (access(P_FIFO, F_OK) == 0)
    {                                     // 判断管道文件是否存在
        execlp("rm", "-f", P_FIFO, NULL); // 删掉
        printf("access.\n");
    }

    if (mkfifo(P_FIFO, 0777) < 0)
    {
        printf("create named pipe failed.\n");
        exit(1);
    }
    fd = open(P_FIFO, O_RDONLY | O_NONBLOCK); // 非阻塞方式打开
    while (1)
    {
        memset(cache, 0, sizeof cache);
        if ((read(fd, cache, 100)) == 0)
        {
            printf("no data:\n");
        }
        else{
            printf("getdata:%s\n",cache);
        }
        sleep(1);
    }

    return 0;
}