#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int fd = open("mytest.txt", O_RDWR | O_CREAT, 0644);
    if (fd < 0)
    {
        perror("open error!");
        exit(1);
    }
    // 申请共享内存
    void *p = mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED){
        perror("mmap error");
        exit(1);
    }

    strcpy((char *)p,"abcefg\0");
    int ret = munmap(p,100); //释放共享映射
    if(-1 ==ret){
        perror("munmap error!");
        exit(1);
    }
    close(fd);


    return 0;
}