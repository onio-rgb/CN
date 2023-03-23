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
int createServerTCP(int port, int listen_val)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sfd, listen_val);
    return sfd;
}
int main()
{
    int server_fd = createServerTCP(9010, 2);
    while (1)
    {
        struct sockaddr client_addr;
        socklen_t client_len;
        int nsfd = accept(server_fd, &client_addr, &client_len);

        char msg[100];
        sprintf(msg, "This is message from live telecast\n");
        send(nsfd, msg, 100, 0);
        close(nsfd);
    }
}