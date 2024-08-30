#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>

#define SHARED_PATH "/tmp/test"

union semun
{
    int val;               /* Value for SETVAL */
    struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* Array for GETALL, SETALL */
    struct seminfo *__buf; /* Buffer for IPC_INFO
                              (Linux-specific) */
};

int sem_init_value(int id, int idx, int value)
{
    union semun un;
    un.val = value;
    int ret = semctl(id, idx, SETVAL, un);
    if (-1 == ret)
    {
        perror("semctl setval");
        return -1;
    }
    printf("sem init value:%d\n", value);
    return 0;
}

int sem_get_value(int id, int idx)
{
    union semun un;
    int ret = semctl(id, idx, GETVAL, un); // 返回信号量值
    if (-1 == ret)
    {
        perror("semctl setval");
        return -1;
    }
    printf("sem get value:%d\n", ret);
    return 0;
}

int sem_p(int id)
{
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg |= SEM_UNDO;
    int ret = semop(id, &buf, 1); // 第三个参数是数组长度
    if (-1 == ret)
    {
        perror("sem w");
        return -1;
    }
    return 0;
}

int sem_w(int id)
{
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 0;
    buf.sem_flg |= SEM_UNDO;
    int ret = semop(id, &buf, 1); // 第三个参数是数组长度
    if (-1 == ret)
    {
        perror("sem v");
        return -1;
    }
    return 0;
}

int sem_v(int id)
{
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg |= SEM_UNDO;
    int ret = semop(id, &buf, 1); // 第三个参数是数组长度
    if (-1 == ret)
    {
        perror("sem p");
        return -1;
    }
    return 0;
}

// 和信号量结合进行同步测试
int main(int argc, char **argv)
{
    int mode = atoi(argv[1]);
    int val = atoi(argv[2]);
#if 1
    // 创建键值
    key_t semkey = ftok("/tmp/test", 2);
    if (-1 == semkey)
    {
        perror("ftok");
        return -1;
    }

    // 创建一个信号量
    int semid = semget(semkey, 5, 0644 | IPC_CREAT);
    printf("semid:%d\n", semid);

    // 初始化
    if (val >= 0)
        sem_init_value(semid, 0, val);
    sem_get_value(semid, 0);
#endif

#if 1

    // 创建键值
    key_t shmkey = ftok(SHARED_PATH, 1);
    if (-1 == shmkey)
    {
        perror("ftok");
        return -1;
    }
    // 分配内存段
    int shmid = shmget(shmkey, 1024, 0644 | IPC_CREAT);
    // 通过内存段标识获取所在位置
    void *addr = shmat(shmid, NULL, 0);
    if (addr == (void *)-1)
    {
        perror("shmat fail..");
        return -1;
    }
    int i = 0;
    while (1)
    {
        if (0 == mode)
        { // 只读
            sem_p(semid);  //P操作
            char rbuf[1024] = {0};
            memcpy(rbuf, addr, 1024);
            printf("rbuf:%s\n", rbuf);
        }
        else // 写进程
        {
            char wbuf[1024] = {0};
            sprintf(wbuf, "%d", i++);
            memset(addr, 0, 1024); // 清空共享内存
            memcpy(addr, wbuf, strlen(wbuf));
            printf("succsss\n");
            sem_v(semid); //V操作
            sleep(1);
        }
    }
    // 删除共享内存
    shmctl(shmid, IPC_RMID, NULL);

#endif
    return 0;
}
