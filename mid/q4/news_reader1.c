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
int live_tele_fd;
int screen_fd;
void pause_exec(int sig)
{
    printf("Paused reading : News reader 1\n");
    pause();
}
void resume_exec(int sig)
{
    printf("Resumed reading : News reader 1\n");
}
void handleLiveTelecast(int port)
{
    printf("%d",port);
    int fd = fileno(popen("pidof ./news2", "r"));
    char s[1000];
    read(fd, &s, 1000);
    int X = atoi(s);
    printf("sasas%d\n",X);
    kill(X, SIGUSR1);

    live_tele_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in live_tele_addr;
    live_tele_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    live_tele_addr.sin_family = AF_INET;
    live_tele_addr.sin_port = htons(9010);
    connect(live_tele_fd, (struct sockaddr *)&live_tele_addr, sizeof(live_tele_addr));
    reuseaddr(live_tele_fd);
    
    printf("Message from Live Telecast : \n");
    char buff[100];
    recv(live_tele_fd, buff, 100, 0);
    send(screen_fd, buff, 100, 0);

    kill(X, SIGCONT);
}
int check(char *buff)
{
    int i = 0;
    int port = 0;
    while (buff[i] >= '0' && buff[i] <= '9')
    {
        port = port * 10 + buff[i] - '0';
        ++i;
    }
    if (port != 0)
    {
        return port;
    }
    else
        return -1;
}
void reuseaddr(int fd)
{
    int *val = malloc(sizeof(int));
    *val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, val, sizeof(int));
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, val, sizeof(int));
}
int main()
{
    printf("Pid 1 : %ld\n",getpid());
    struct sigaction act;
    act.sa_handler = &pause_exec;
    sigaction(SIGUSR1, &act, NULL);
    act.sa_handler = &resume_exec;
    sigaction(SIGUSR2, &act, NULL);


    screen_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in screen_addr;
    screen_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    screen_addr.sin_family = AF_INET;
    screen_addr.sin_port = htons(9000);
    connect(screen_fd, (struct sockaddr *)&screen_addr, sizeof(screen_addr));
    reuseaddr(screen_fd);

    int editor_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in e_addr;
    e_addr.sin_addr.s_addr = INADDR_ANY;
    e_addr.sin_port = htons(8090);
    e_addr.sin_family = AF_INET;
    reuseaddr(editor_fd);
    bind(editor_fd, (struct sockaddr *)&e_addr, sizeof(e_addr));
    listen(editor_fd, 1);
    struct sockaddr client_addr;
    socklen_t client_len;
    int nsfd = accept(editor_fd, &client_addr, &client_len);
    while (1)
    {
        char buff[100];
        recv(nsfd, buff, 100, 0);

        int port = check(buff);
        if (port != -1)
        {
            handleLiveTelecast(port);
        }

        char add_reader_name[200];
        sprintf(add_reader_name, "%s From news reader 1\n", buff);
        send(screen_fd, add_reader_name, 200, 0);
    }
}