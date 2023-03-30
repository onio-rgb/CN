#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdlib.h>
int connectToTCP(int port)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    perror("");
    return sfd;
}
int main()
{
    int sfd = connectToTCP(8080);
    int seq[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int *train_no = malloc(sizeof(int));
    for (int i = 0; i < 3; i++)
    {
        *train_no = i;
        send(sfd, train_no, sizeof(int), 0);
        send(sfd, seq, sizeof(int) * 10, 0);
        sleep(1);
    }
}