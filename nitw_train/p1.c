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

    if (recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC) < 0)
        return -1;

    if (message_buffer[0] != 'F')
    {
        /* this did not originate from the above function */
        return -1;
    }

    if ((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
    {
        /* we did not provide enough space for the ancillary element array */
        return -1;
    }

    /* iterate ancillary elements */
    for (control_message = CMSG_FIRSTHDR(&socket_message);
         control_message != NULL;
         control_message = CMSG_NXTHDR(&socket_message, control_message))
    {
        if ((control_message->cmsg_level == SOL_SOCKET) &&
            (control_message->cmsg_type == SCM_RIGHTS))
        {
            sent_fd = *((int *)CMSG_DATA(control_message));
            return sent_fd;
        }
    }

    return -1;
}

struct shmState
{
    int arrived;
    int leaving;
    int platform_available[3];
};
struct shmState *state;
void checkArrivedTrain(int sig)
{
    printf("Platform 1 : Train %d arrived \n", state->arrived);
}
int main()
{
    struct sigaction act;
    act.sa_handler = &checkArrivedTrain;
    sigaction(SIGUSR1, &act, NULL);

    key_t key = ftok("/shm", 100);
    perror("");
    int shm_id = shmget(key, 4096, 0666);

    state = shmat(shm_id, NULL, 0);

    int usfd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un userv_addr;
    userv_addr.sun_family = AF_UNIX;
    strcpy(userv_addr.sun_path, "plat1");
    bind(usfd, (struct sockaddr *)&userv_addr, sizeof(userv_addr));
    listen(usfd, 20);

    struct sockaddr client_addr;
    socklen_t client_len;
    int nusfd = accept(usfd, &client_addr, &client_len);

    while (1)
    {
        int recvfd=recv_fd(nusfd);
        int seq[50];
        read(recvfd,seq,sizeof(int)*50);
        printf("Sequence of compartments in train \n");
        
    }
}