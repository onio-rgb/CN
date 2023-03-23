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
void connectToTCP(int port, char *ip, int csfd)
{
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);
    server_addr.sin_family = AF_INET;
    connect(csfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
}
int main()
{
    char c;
    scanf("%c", &c);
    int reporters_fd[3];
    for (int i = 0; i < 3; i++)
    {
        reporters_fd[i] = socket(AF_INET, SOCK_STREAM, 0);
        connectToTCP(8080 + i, "127.0.0.1", reporters_fd[i]);
    }
    for (int i = 0; i < 3; i++)
    {
        char msg[100];
        if (i != 0)
            sprintf(msg, "Reporter %d : This happened ", i + 1);
        else
            sprintf(msg, "9010 Reporter %d : This happened ", i + 1);
        send(reporters_fd[i], msg, 100, 0);
    }
}