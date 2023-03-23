#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <sys/msg.h>
#include <errno.h>
#include <sys/select.h>
int news_flag = 0;
int news_readerfds[2];
void *readNewsToNewsReader(void *arg)
{
    int reporter_fd = *(int *)arg;
    char buff[100];
    recv(reporter_fd, buff, 100, 0);
    if (buff[0] == '/' && buff[1] == 'd')
    {
    }
    send(news_readerfds[news_flag], buff, 100, 0);
    news_flag = (news_flag + 1) % 2;
}
int main()
{
    int reporter_fds[3];
    fd_set readfds;
    FD_ZERO(&readfds);
    for (int i = 0; i < 3; i++)
    {
        reporter_fds[i] = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in r_addr;
        r_addr.sin_addr.s_addr = INADDR_ANY;
        r_addr.sin_port = htons(8080 + i);
        r_addr.sin_family = AF_INET;
        bind(reporter_fds[i], (struct sockaddr *)&r_addr, sizeof(r_addr));
        listen(reporter_fds[i], 1);
        int *val = malloc(sizeof(int));
        *val = 1;
        setsockopt(reporter_fds[i], SOL_SOCKET, SO_REUSEPORT, val, sizeof(int));
        setsockopt(reporter_fds[i], SOL_SOCKET, SO_REUSEADDR, val, sizeof(int));
        FD_SET(reporter_fds[i], &readfds);
    }
    for (int i = 0; i < 2; i++)
    {
        news_readerfds[i] = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in news_reader_addr;
        news_reader_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        news_reader_addr.sin_family = AF_INET;
        news_reader_addr.sin_port = htons(8090 + i);
        connect(news_readerfds[i], (struct sockaddr *)&news_reader_addr, sizeof(news_reader_addr));
        int *val = malloc(sizeof(int));
        *val = 1;
        setsockopt(news_readerfds[i], SOL_SOCKET, SO_REUSEPORT, val, sizeof(int));
        setsockopt(news_readerfds[i], SOL_SOCKET, SO_REUSEADDR, val, sizeof(int));
        perror("");
    }
    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            if (FD_ISSET(reporter_fds[i], &readfds))
            {
                struct sockaddr client_addr;
                socklen_t client_len;
                int nsfd = accept(reporter_fds[i], &client_addr, &client_len);
                printf("asas\n");
                pthread_t th;
                int *nsfd_arg = malloc(sizeof(int));
                *nsfd_arg = nsfd;
                pthread_create(&th, NULL, readNewsToNewsReader, nsfd_arg);
            }
        }
        FD_ZERO(&readfds);
        for (int i = 0; i < 3; i++)
            FD_SET(reporter_fds[i], &readfds);
    }
}