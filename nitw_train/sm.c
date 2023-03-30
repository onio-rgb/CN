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
#include <stdlib.h>
#include <signal.h>
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
    *((int *)CMSG_DATA(control_message)) = fd_to_send;

    return sendmsg(socket, &socket_message, 0);
}

struct shmState
{
    int arrived;
    int leaving;
    int platform_available[3];
};
struct shmState *state;
void checkTrainLeaving(int sig)
{
    printf("Train %d is leaving \n", state->leaving);
}
int main()
{
    struct sigaction act;
    act.sa_handler = &checkTrainLeaving;
    act.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &act, NULL);

    key_t key = ftok("/tmp", 100);
    perror("");
    int shm_id = shmget(key, 4096, 0666 | IPC_CREAT);
    perror("");
    state = shmat(shm_id, NULL, 0);
    fd_set readfds;
    FD_ZERO(&readfds);
    // int train_fd[3];
    // for (int i = 0; i < 3; i++)
    // {
    //     train_fd[i] = socket(AF_INET, SOCK_STREAM, 0);
    //     struct sockaddr_in serv_addr;
    //     serv_addr.sin_addr.s_addr = INADDR_ANY;
    //     serv_addr.sin_family = AF_INET;
    //     serv_addr.sin_port = htons(8080 + i);
    //     bind(train_fd[i], (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    //     listen(train_fd[i], 1);
    //     int *val = malloc(sizeof(int));
    //     *val = 1;
    //     setsockopt(train_fd[i], SOL_SOCKET, SO_REUSEADDR, val, sizeof(int));
    //     setsockopt(train_fd[i], SOL_SOCKET, SO_REUSEPORT, val, sizeof(int));
    //     FD_SET(train_fd[i], &readfds);
    // }
    int train_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    int *val = malloc(sizeof(int));
    *val = 1;
    setsockopt(train_fd, SOL_SOCKET, SO_REUSEADDR, val, sizeof(int));
    setsockopt(train_fd, SOL_SOCKET, SO_REUSEPORT, val, sizeof(int));
    bind(train_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(train_fd, 20);

    int usfd_platform[3];
    for (int i = 0; i < 3; i++)
    {
        usfd_platform[i] = socket(AF_UNIX, SOCK_STREAM, 0);
        struct sockaddr_un serv_addr;
        serv_addr.sun_family = AF_UNIX;
        char platform_path[200];
        sprintf(platform_path, "plat%d", i + 1);
        strcpy(serv_addr.sun_path, platform_path);
        connect(usfd_platform[i], (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    }

    struct sockaddr client_addr;
    socklen_t client_len;
    int nsfd = accept(train_fd, &client_addr, &client_len);
    perror("");
    FD_SET(nsfd,&readfds);
    int platform_available[3];
    for (int i = 0; i < 3; i++)
    {
        platform_available[i] = 1;
    }

    while (1)
    {
        int ready=select(1000,&readfds,NULL,NULL,NULL);
        if (FD_ISSET(nsfd, &readfds))
        {
            printf("ASas\n");
            int *train_no = malloc(sizeof(int));
            for (int j = 0;; j = (j + 1) % 3)
            {
                if (platform_available[j] == 1)
                {
                    recv(nsfd, train_no, sizeof(int), 0);
                    printf("trainno %d\n", *train_no);
                    platform_available[j] = 0;
                    // transfer nsfd to platform j
                    printf("%d\n", nsfd);
                    send_fd(usfd_platform[j], nsfd);
                    
                    break;
                }
            }
            state->arrived = *train_no + 1;
            for (int i = 0; i < 3; i++)
            {
                char cmd[200];
                sprintf(cmd, "pidof ./p%d", i + 1);
                int fd = fileno(popen(cmd, "r"));
                char pid[100];
                read(fd, pid, 100);
                int X = atoi(pid);
                printf("%d\n", X);
                kill(X, SIGUSR1);
            }
            FD_ZERO(&readfds);
            FD_SET(nsfd, &readfds);
        }
    }
}