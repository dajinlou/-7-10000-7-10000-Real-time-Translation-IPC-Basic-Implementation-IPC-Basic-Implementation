#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define P_FIFO "/tmp/p_fifo"

int main(int argc,char **argv){
    int fd;
    if(argc<2){
        printf("please input the write data.\n");
        exit(1);
    }
    fd = open(P_FIFO,O_WRONLY|O_NONBLOCK); //非阻塞
    if(-1 == fd){
        printf("open failed!\n");
        return 0;
    }
    else{
        printf("open success!");
    }
    write(fd,argv[1],100);
    close(fd);

    return 0;
}