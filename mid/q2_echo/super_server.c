#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <poll.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <pthread.h>
int sock_fd[4];
int send_fd(int socket, int fd_to_send)
 {
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  /* storage space needed for an ancillary element with a paylod of length is CMSG_SPACE(sizeof(length)) */
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
  int available_ancillary_element_buffer_space;

  /* at least one vector of one byte must be sent */
  message_buffer[0] = 'F';
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;

  /* initialize socket message */
  memset(&socket_message, 0, sizeof(struct msghdr));
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
  memset(ancillary_element_buffer, 0, available_ancillary_element_buffer_space);
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = available_ancillary_element_buffer_space;

  /* initialize a single ancillary data element for fd passing */
  control_message = CMSG_FIRSTHDR(&socket_message);
  control_message->cmsg_level = SOL_SOCKET;
  control_message->cmsg_type = SCM_RIGHTS;
  control_message->cmsg_len = CMSG_LEN(sizeof(int));
  *((int *) CMSG_DATA(control_message)) = fd_to_send;

  return sendmsg(socket, &socket_message, 0);
 }
int connectToUnix()
{
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un serv_addr;
    serv_addr.sun_family = AF_UNIX;
    char path[200];
    sprintf(path, "unix");
    strcpy(serv_addr.sun_path, path);
    connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    return sfd;
}
int createUnixServer()
{
    int usfd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un userv_addr;
    userv_addr.sun_family = AF_UNIX;
    strcpy(userv_addr.sun_path, "unix");
    bind(usfd, (struct sockaddr *)&userv_addr, sizeof(userv_addr));
    listen(usfd, 20);
    return usfd;
}
int nusfd[4];
void *handleEcho(void *arg)
{
    int usfd = createUnixServer();
    fd_set readfd;
    FD_ZERO(&readfd);
    FD_SET(usfd, &readfd);
    int i = 0;
    while (i < 4)
    {
        int ready = select(1000, &readfd, NULL, NULL, NULL);
        printf("%d\n", ready);
        if (FD_ISSET(usfd, &readfd))
        {
            struct sockaddr client_addr;
            socklen_t client_len;
            int _nusfd = accept(usfd, &client_addr, &client_len);
            nusfd[i++] = _nusfd;
            send_fd(_nusfd,sock_fd[i-1]);
            FD_ZERO(&readfd);
            FD_SET(usfd, &readfd);
        }
    }

    fd_set readfds;
    for (int i = 0; i < 4; i++)
    {
        FD_SET(nusfd[i], &readfds);
    }
    while (1)
    {
        int ready = select(1000, &readfds, NULL, NULL, NULL);
        for (int i = 0; i < 4; i++)
        {
            if (FD_ISSET(nusfd[i], &readfds))
            {
                char buff[100];
                recv(nusfd[i], buff, 100, 0);
                printf("%sdsfdf\n", buff);
            }
        }
        for (int i = 0; i < 4; i++)
        {
            FD_SET(nusfd[i], &readfds);
        }
    }
}
int main()
{

    pthread_t th;
    pthread_create(&th, NULL, handleEcho, NULL);

    struct pollfd *fds = malloc(sizeof(struct pollfd) * 4);
    for (int i = 0; i < 4; i++)
    {
        sock_fd[i] = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in server_addr;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(8080 + i);
        int k = 1;
        setsockopt(sock_fd[i], SOL_SOCKET, SO_REUSEADDR, &k, sizeof(int));
        setsockopt(sock_fd[i], SOL_SOCKET, SO_REUSEPORT, &k, sizeof(int));
        bind(sock_fd[i], (struct sockaddr *)&server_addr, sizeof(server_addr));
        listen(sock_fd[i], 20);
        fds[i].fd = sock_fd[i];
        fds[i].events = POLL_IN;
    }
    for (int i = 0; i < 4; i++)
    {
        int c = fork();
        if (c == 0)
        {
            char *arg[1] = {NULL};
            char path[10];
            sprintf(path, "./s%d", i + 1);
            execvp(path, arg);
            printf("Execv failed\n");
        }
    }
    while(1);
    
}