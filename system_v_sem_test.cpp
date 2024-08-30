#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <string.h>

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

int main(int argc, char **argv)
{

    int op = atoi(argv[1]);
    int val = atoi(argv[2]);

    // 创建键值
    key_t key = ftok("/tmp/test", 1);
    if (-1 == key)
    {
        perror("ftok");
        return -1;
    }

    // 创建一个信号量
    int id = semget(key, 5, 0644 | IPC_CREAT);
    printf("id:%d\n", id);

    // 初始化
    if (val >= 0)
        sem_init_value(id, 0, val);
    sem_get_value(id, 0);

    if (-1 == op)
    { // 申请资源
        sem_p(id);
    }
    else if (0 == op)
    {
        sem_w(id);
    }
    else
    {
        sem_v(id);
    }

    sem_get_value(id, 0);
    // pause();
    sleep(5);
    union semun un;
    semctl(id,0,IPC_RMID,un);
    return 0;
}
