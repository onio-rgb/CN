#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
void *handleClient(void *arg)
{
    int client_fd = *(int *)arg;
    char buff[100];
    sprintf(buff,"Service 1 : Served client %d\n", client_fd);
    send(client_fd, buff, 100, 0);
    close(client_fd);
}
int main(int argc, char *argv[])
{
    int sock_fd = atoi(argv[0]);  
    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = 0;
        int client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        perror("");
        pthread_t th;
        int *client_fd_arg = malloc(sizeof(int));
        *client_fd_arg = client_fd;
        pthread_create(&th, NULL, handleClient, client_fd_arg);
    }
}