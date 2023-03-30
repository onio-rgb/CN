#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h>
 #include <pthread.h>
#include <errno.h>
int usfd;
void *handleClient(void *arg)
{
    int client_fd = *(int *)arg;
    char buff[100];
    sprintf(buff,"Service 2 : Served client %d", client_fd);
    send(usfd,buff,100,0);
    send(client_fd, buff, 100, 0);
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
int recv_fd(int socket)
 {
  int sent_fd, available_ancillary_element_buffer_space;
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];

  /* start clean */
  memset(&socket_message, 0, sizeof(struct msghdr));
  memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));

  /* setup a place to fill in message contents */
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = CMSG_SPACE(sizeof(int));

  if(recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC) < 0)
   return -1;

  if(message_buffer[0] != 'F')
  {
   /* this did not originate from the above function */
   return -1;
  }

  if((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
  {
   /* we did not provide enough space for the ancillary element array */
   return -1;
  }

  /* iterate ancillary elements */
   for(control_message = CMSG_FIRSTHDR(&socket_message);
       control_message != NULL;
       control_message = CMSG_NXTHDR(&socket_message, control_message))
  {
   if( (control_message->cmsg_level == SOL_SOCKET) &&
       (control_message->cmsg_type == SCM_RIGHTS) )
   {
    sent_fd = *((int *) CMSG_DATA(control_message));
    return sent_fd;
   }
  }

  return -1;
 }
int main(int argc, char *argv[])
{
    usfd=connectToUnix();
    int sock_fd = recv_fd(usfd);  
    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = 0;
        int client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        perror("");
        pthread_t th;
        int *client_fd_arg = malloc(sizeof(int));
        *client_fd_arg = client_fd;
        pthread_create(&th, NULL, handleClient, client_fd_arg);
    }
}