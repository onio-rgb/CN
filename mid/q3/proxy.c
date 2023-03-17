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
int server_fds[5]; // sock fd for communication b/w proxy and 5 special servers
int msgid;
struct msgbuf
{
    long mtype;
    int client_id;
};
void *handleClientServer(void *arg)
{
    int service_no = *(int *)arg;
    while (1)
    {
        struct msgbuf recvmsg;
        int client_fd;
        msgrcv(msgid, &recvmsg, sizeof(int), service_no, 0); // service_no is 1 indexed
        client_fd=recvmsg.client_id;
        char *buff = malloc(100);
        // sprintf(buff,"Message from client to service %d\n",service_no);
        recv(client_fd, buff, 100, 0);
        printf("received from client \n");
        send(server_fds[service_no - 1], buff, 100, 0);
        printf("sent to server %d \n",service_no);
        buff = malloc(100);
        recv(server_fds[service_no - 1], buff, 100, 0);
        printf("recieved from server %d \n",service_no);
        send(client_fd, buff, 100, 0);
        printf("sent vack to client \n");
        close(client_fd);
    }
}
int main()
{
    key_t key = ftok("/home/onio", 100);
    msgid = msgget(key, 0666);
    msgctl(msgid, IPC_RMID, NULL);
    msgid = msgget(key, 0666|IPC_CREAT);
    int proxy_fd = socket(AF_INET, SOCK_STREAM, 0); // fd for clients to communicate with proxy
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9090);
    bind(proxy_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(proxy_fd, 20);
    pthread_t ths[5]; // threads for dealing with servers
    for (int i = 0; i < 5; i++)
    {
        server_fds[i] = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in server_addr;
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(8080 + i);

        connect(server_fds[i], (struct sockaddr *)&server_addr, sizeof(server_addr));
        int *index = malloc(sizeof(int));
        *index = i + 1;
        
        pthread_create(&ths[i], NULL, handleClientServer, index);
    }
    while (1)
    {
        struct sockaddr client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(proxy_fd, &client_addr, &len);
        int server_no = 0;
        recv(client_fd, &server_no, sizeof(int), 0);

        struct msgbuf msg;
        msg.mtype = server_no;
        msg.client_id = client_fd;

        msgsnd(msgid, &msg, sizeof(int), 0);
    }
}
