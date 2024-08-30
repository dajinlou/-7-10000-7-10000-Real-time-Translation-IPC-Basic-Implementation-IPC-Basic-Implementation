#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>

const int MODE_RECEIVE = 0;
const int MODE_SEND = 1;
int do_notify(int fd);
void test_proc(sigval_t val)
{
    int fd = val.sival_int;
    do_notify(fd);
    char rbuf[2048] = {0};
    unsigned int priority = 0;
    int ret = mq_receive(fd, rbuf, sizeof(rbuf), &priority);
    if (-1 == ret)
    {
        perror("receive error");
        return;
    }
    printf("ret:%d, priority:%u,data:%s\n", ret, priority, rbuf);
}

int do_notify(int fd)
{
    struct sigevent ev;
    ev.sigev_value.sival_int = fd;
    ev.sigev_notify = SIGEV_THREAD;
    ev.sigev_notify_function = test_proc;
    ev.sigev_notify_attributes = NULL;

    int ret = mq_notify(fd, &ev);
    if (-1 == ret)
    {
        perror("mq_notify error");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{

    int mode = atoi(argv[1]);
    if (mode == -1)
    {
        mq_unlink("/test1");
        return 0;
    }
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1500;

    // 打开消息队列
    mqd_t fd = mq_open("/test1", O_RDWR | O_CREAT, 0644, &attr);
    if (-1 == fd)
    {
        perror("mq_open error");
        return -1;
    }
    if (MODE_RECEIVE == mode)
    {
        do_notify(fd);
    }
    while ((1))
    {
        if (MODE_RECEIVE == mode)
        { // 读模式
            sleep(1);
#if 0
            char rbuf[2048] = {0};
            unsigned int priority = 0;
            int ret = mq_receive(fd, rbuf, sizeof(rbuf), &priority);
            if (-1 == ret)
            {
                perror("receive error");
                break;
            }
            printf("ret:%d, priority:%u,data:%s\n", ret, priority, rbuf);
#endif
        }
        else
        {
#define TEST_STRING "123456789"
            int ret = mq_send(fd, TEST_STRING, strlen(TEST_STRING), 1);
            if (-1 == ret)
            {
                perror("send error");
                break;
            }
            sleep(1);
        }
    }

    return 0;
}