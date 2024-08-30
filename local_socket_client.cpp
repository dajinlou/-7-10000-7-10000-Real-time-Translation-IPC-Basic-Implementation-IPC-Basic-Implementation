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
    int cfd;
    struct sockaddr_un servAddr;

    cfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (-1 == cfd)
    {
        perror("create cfd fail...");
        exit(EXIT_FAILURE);
    }

    bzero(&servAddr, sizeof(servAddr));
    servAddr.sun_family = AF_LOCAL;
    strcpy(servAddr.sun_path, UNIXSTR_PATH);

    int ret = connect(cfd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if (-1 == ret)
    {
        perror("connect fail..");
        exit(EXIT_FAILURE);
    }

    char buf[BUFFER_SIZE];
    while (1)
    {
        bzero(buf, sizeof buf);
        printf(">>");
        if (fgets(buf, BUFFER_SIZE, stdin) == NULL)
        {
            break;
        }
        write(cfd, buf, strlen(buf));
    }
    close(cfd);

    return 0;
}