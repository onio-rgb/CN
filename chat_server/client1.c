#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <poll.h>
#define CLIENT_NO 1
int errno;
struct msg
{
    int type;
    int client_no;
    char msg[200];
};
void *receive_msg(void *arg)
{
    char *fifo_id = malloc(200);
    sprintf(fifo_id, "/home/onio/client%d", CLIENT_NO);
    int client_fd = open(fifo_id, O_RDONLY);
    while (1)
    {
        struct msg msg_rcv;
        read(client_fd, &msg_rcv, sizeof(msg_rcv));
        // perror("");
        printf("%s\n", msg_rcv.msg);
    }
}
int main()
{
    int ffd = open("/home/onio/famous", O_WRONLY);
    struct msg msg_req;
    msg_req.client_no = CLIENT_NO;
    msg_req.type = 0;
    write(ffd, &msg_req, sizeof(msg_req));
    pthread_t read_msg_thread;
    pthread_create(&read_msg_thread, NULL, receive_msg, NULL);
    while (1)
    {
        struct msg msg_snd;
        msg_snd.type = 1;
        msg_snd.client_no = CLIENT_NO;
        sprintf(msg_snd.msg, "Msg from client %d", CLIENT_NO);
        write(ffd, &msg_snd, sizeof(msg_snd));
        sleep(4 + CLIENT_NO);
    }
}