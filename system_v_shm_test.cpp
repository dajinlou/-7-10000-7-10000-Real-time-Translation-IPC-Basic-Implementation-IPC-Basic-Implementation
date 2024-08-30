#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define SHARED_PATH "/tmp/test"
int main(int argc,char** argv){
#if 1
    int mode = atoi(argv[1]);

    //创建键值
    key_t shmkey = ftok(SHARED_PATH,1);
    if(-1 == shmkey){
        perror("ftok");
        return -1;
    }
    //分配内存段
    int shmid = shmget(shmkey,1024,0644|IPC_CREAT);
    //通过内存段标识获取所在位置
    void* addr = shmat(shmid,NULL,0);
    if(addr == (void*)-1){
        perror("shmat fail..");
        return -1;
    }
    int i=0;
    while(1){
        if(0==mode){ //只读
            char rbuf[1024]={0};
            memcpy(rbuf,addr,1024);
            printf("rbuf:%s\n",rbuf);
        }
        else{
            char wbuf[1024]={0};
            sprintf(wbuf,"%d",i++);
            memset(addr,0,1024); //清空共享内存
            memcpy(addr,wbuf,strlen(wbuf));
            printf("succsss\n");
            sleep(1);
        }
    }
    //删除共享内存
    shmctl(shmid,IPC_RMID,NULL);

#endif
    return 0;
}
