#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>
#include<signal.h>
int self_pid;
int left = -1;
int right = -1;
struct msgbuf
{
    long mtype;
    char mtext[200];
};
void form_group(int sig)
{
    key_t key = ftok("/tmp/Q", 100);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    struct msgbuf msgp;
    for (int i = 0; i < 3; i++)
    {
        msgrcv(msgid, &msgp, 200, 18, 0);
        if (msgp.mtype == 18)
        {
            char *pid_string = malloc(10);
            strncpy(pid_string, &msgp.mtext[2], strlen(msgp.mtext) - 2);
            int pid_int = atoi(pid_string);
            setpgid(pid_int, self_pid);
            printf("pgid of %d is %d\n", pid_int, getpgid(pid_int));
        }
    }
}
void receive_msg_msgq()
{
    key_t key = ftok("/tmp/Q", 100);
    int msgid = msgget(key, 0666);
    if (msgid == -1)
    {
        printf("message queue not created\n");
        fflush(stdout);
        exit(1);
    }
    struct msgbuf msgp;
    msgrcv(msgid, &msgp, 200, 2, 0);
    if (msgp.mtype == 2)
    {
        char *pid_string = malloc(10);
        strncpy(pid_string, &msgp.mtext[2], strlen(msgp.mtext) - 2);
        int pid_int = atoi(pid_string);
        right = pid_int;
    }
}
int main()
{
    // receive msg from p2 type=2
    receive_msg_msgq();
    // send a signal to p2
    kill(right, SIGUSR1);
    // receive signal from p4 to form groups
    self_pid = getpid();
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = &form_group;
    sigaction(SIGUSR1, &act, NULL);
    sleep(10);
}