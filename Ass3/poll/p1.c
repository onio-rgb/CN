#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>

int main()
{
    struct pollfd *poll_fds = malloc(sizeof(struct pollfd) * 4);
    FILE **fstreams = malloc(sizeof(FILE *) * 4);
    for (int i = 2; i <= 5; i++)
    {
        char command[10];
        sprintf(command, "./p%d", i);
        FILE *pipein_stream = popen(command, "r");
        fstreams[i - 2] = pipein_stream;
        if (pipein_stream == NULL)
        {
            printf("Errpr popen p%d\n", i);
            fflush(stdout);
            return i;
        }
        int pipe_in_fd = fileno(pipein_stream);
        poll_fds[i-2].fd = pipe_in_fd;
        poll_fds[i-2].events = POLLIN;
    }
    int num_open_fds = 4;
    int nfd = 4;
    int it = 4;
    // signal(SIGPIPE,handle_sigpipe);
    while (num_open_fds > 0 && it > 0)
    {
        int ready = poll(poll_fds, 4, 3000);
        if (ready == -1)
        {
            printf("poll\n");
            fflush(stdout);
            return 10;
        }
        if (ready == 0)
        {
            it--;
            continue;
        }
        for (int i = 0; i < nfd; i++)
        {
            if (poll_fds[i].revents & POLLIN)
            {
                int stdin_cpy = dup(0);
                int pipe_in_fd = poll_fds[i].fd;
                dup2(pipe_in_fd, 0);
                char buff[100];
                fflush(stdout);
                scanf(" %[^\n]", buff);
                
                // writing to p6 from p2 p3 p4 p5
                FILE *p6_stream = popen("./p6", "w");
                int stdout_cpy = dup(1);
                int p6_fd = fileno(p6_stream);
                dup2(p6_fd, 1);
                printf("%s\n", buff);
                fflush(stdout);
                // resetting I/O
                dup2(stdout_cpy, 1);
                dup2(stdin_cpy, 0);
                // closing popens
                pclose(fstreams[i]);
                pclose(p6_stream);
                poll_fds[i].revents=0;
                poll_fds[i].events=0;
                poll_fds[i].fd=-1;

            }
        }
        num_open_fds-=ready;
    }
    fflush(stdout);
    if (num_open_fds == 4)
    {
        // writing to p6 from p2 p3 p4 p5
        FILE *p6_stream = popen("./p6", "w");
        int stdout_cpy = dup(1);
        int p6_fd = fileno(p6_stream);
        dup2(p6_fd, 1);
        printf("None of p2 p3 p4 p5 responded so p1 replying\n");
        fflush(stdout);
        // resetting I/O
        dup2(stdout_cpy, 1);
    }
    // sleep(10);
}