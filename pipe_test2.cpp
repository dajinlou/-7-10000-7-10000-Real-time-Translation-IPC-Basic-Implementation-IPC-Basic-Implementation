#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

//实现子写父亲读

int main()
{

    int fd[2]; // 0:读(子) 1:写（父）
    pid_t pid;

    int ret = pipe(fd);
    if (ret == -1)
    {
        perror("pipe error!");
        exit(1);
    }
    pid = fork(); //有两次返回
    if (-1 == pid)
    {
        perror("create fail....");
        exit(1);
    }
    else if (pid == 0)
    {
        // 子进程 写1
        sleep(1);
        close(fd[0]);
        printf("child=%d\n", getpid());
        write(fd[1], "hello pipe\n", 11);
    }
    else if (pid > 0)
    {
          // 父进程 读
        close(fd[1]);
        char buf[1024];
        printf("father=%d\n", getpid());
        ret = read(fd[0], buf, sizeof buf);
        write(STDOUT_FILENO, buf, ret);
        wait(NULL); //回收子进程，避免成为僵尸进程
    }
    printf("finished:%d\n",getpid());
    return 0;
}