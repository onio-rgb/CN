#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
int errno;
struct msg
{
    int type;
    int client_no;
    char msg[200];
};
int main()
{
    unlink("/home/onio/famous");
    mkfifo("/home/onio/famous", 0666);
    int ffd = open("/home/onio/famous", O_RDONLY);
    perror("");
    struct pollfd pfds[1];
    pfds[0].events = POLLIN;
    pfds[0].fd = ffd;
    int client_fds[100];
    int num_clients = 0;
    while (1)
    {
        int ready = poll(pfds, 1, -1);
        if (ready < 0)
        {
            perror("");
            exit(EXIT_FAILURE);
        }
        struct msg r;
        read(pfds[0].fd, &r, sizeof(r));
        if (r.type == 0)
        {
            printf("reg req\n");
            char *fifo_id = malloc(200);
            sprintf(fifo_id, "/home/onio/client%d", r.client_no);
            unlink(fifo_id);
            mkfifo(fifo_id, 0666);
            int client_fd = open(fifo_id, O_WRONLY);
            client_fds[num_clients++] = client_fd;
        }
        else if (r.type == 1)
        {
            printf("msg req\n");
            for (int i = 0; i < num_clients; i++)
            {
                struct msg snd_r = r;
                write(client_fds[i], &snd_r, sizeof(snd_r));
            }
        }
        pfds[0].revents = 0;
    }
}