#include <stdio.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>

#define EXIT_FAILURE 1
#define UNIXSTR_PATH "/tmp/unix.str"
#define LISTENQ 5
#define BUFFER_SIZE 256

int main(void)
{
    int lfd;
    int cfd;
    socklen_t len;
    struct sockaddr_un servAddr;
    struct sockaddr_un cliendAddr;

    lfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (-1 == lfd)
    {
        perror("create lfd fail...");
        exit(EXIT_FAILURE);
    }

    unlink(UNIXSTR_PATH);
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sun_family = AF_LOCAL;
    strcpy(servAddr.sun_path, UNIXSTR_PATH);
    if (-1 == bind(lfd, (struct sockaddr *)&servAddr, sizeof(servAddr)))
    {
        perror("bind failed...");
        exit(EXIT_FAILURE);
    }

    listen(lfd, LISTENQ);
    printf("server started success\n");
    len = sizeof(cliendAddr);
    cfd = accept(lfd, (struct sockaddr *)&cliendAddr, &len);
    if (-1 == cfd)
    {
        perror("accept fail..");
        exit(EXIT_FAILURE);
    }

    char buf[BUFFER_SIZE];
    while (1)
    {
        bzero(buf, sizeof buf);
        if (read(cfd, buf, BUFFER_SIZE) == 0)
            break;
        printf("recv:%s", buf);
    }
    close(lfd);
    close(cfd);
    unlink(UNIXSTR_PATH);
    return 0;
}