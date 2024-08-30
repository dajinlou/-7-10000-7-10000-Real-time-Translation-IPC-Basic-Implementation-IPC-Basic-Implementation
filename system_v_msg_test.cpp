#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// #define __USE_GNU // struct msgbuf
int main(int argc, char **argv)
{
    int mode = atoi(argv[1]);
    int mtype = atoi(argv[2]);
    char* str = argv[3];
    // 生成键值
    // key_t key = 1234;

    //第二种方法
    key_t key = ftok("/tmp/test",1); //文件必须存在
    if(-1 == key){
        perror("ftok");
        return -1;
    }

    // 创建消息队列
    int id = msgget(key, 0644 | IPC_CREAT);
    printf("id:%d\n", id);

    while (1)
    {
        if (0 == mode)
        { // 读模式
            struct msgbuf msg = {0};
            int ret = msgrcv(id, &msg, 1024, mtype, 0);
            if (-1 == ret)
            {
                perror("msgrcv");
                break;
            }
            printf("ret:%d,type:%lu,mtext:%s\n", ret, msg.mtype, msg.mtext);
        }
        else{
            struct  msgbuf msg = {0};
            msg.mtype = mtype;
            memcpy(msg.mtext,str,strlen(str));
            int ret = msgsnd(id,&msg,strlen(str),0);
            if(-1 == ret){
                perror("msgsnd");
                break;
            }
            #if 1 //打印信息
                struct  msqid_ds stat = {0};
                msgctl(id,IPC_STAT,&stat);
                printf("msg queue nums:%lu,bytes:%lu\n",stat.msg_qnum,stat.__msg_cbytes);
                
            #endif
            sleep(1);
        }
    }
    msgctl(id,IPC_RMID,0); //删除消息队列
    return 0;
}