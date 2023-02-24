#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include<sys/select.h>
#include<errno.h>
int errno;
int main()
{
    fd_set read_fd;
    FD_ZERO(&read_fd);
    FILE **fstreams = malloc(sizeof(FILE *) * 4);
    int fds[4];
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
        fds[i-2]=pipe_in_fd;
        FD_SET(pipe_in_fd,&read_fd);
    }
    int num_open_fds = 4;
    int nfd = 4;
    int it = 4;
    // signal(SIGPIPE,handle_sigpipe);
    while (num_open_fds > 0 && it > 0)
    {
        struct timeval tv;
        tv.tv_sec=3;
        tv.tv_usec=0;
        int ready = select(4,&read_fd,NULL,NULL,&tv);
        if (ready == -1)
        {
            printf("%d\n",errno);
            fflush(stdout);
            return 10;
        }
        if (ready == 0)
        {
            it--;
            continue;
        }
        fd_set cpy=read_fd;
        for (int i = 0; i < nfd; i++)
        {
            if (FD_ISSET(fds[i],&read_fd))
            {
                int stdin_cpy = dup(0);
                int pipe_in_fd = fds[i];
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
            }
        }
        num_open_fds-=ready;
        FD_ZERO(&read_fd);
        for(int j=0;j<4;j++){
            if(FD_ISSET(fds[j],&cpy));
            else
                FD_SET(fds[j],&read_fd);
        }
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