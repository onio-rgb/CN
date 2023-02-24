#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>
struct msgbuf
{
    long mtype;
    char mtext[200];
};
int left = -1, right = -1;
void handler(int sig, siginfo_t *info, void *ucontext)
{
    left = info->si_pid;
    return;
}
void signal_handler_connection()
{
    struct sigaction act;
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = &handler;
    if (sigaction(SIGUSR1, &act, NULL) == -1)
    {
        printf("Sigaction p1\n");
        exit(1);
    }
}
int msgid;
void receive_msg_msgq()
{
    key_t key = ftok("/tmp/Q", 100);
    msgid = msgget(key, 0666);
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
void circular_signal(int sig)
{
    char *c="P1 received sigusr1\n";
    write(1,"P1 received sigusr1\n",strlen(c));
    fflush(stdout);
}
void reverse_circular_signal(int sig)
{
    char *c="P1 received sigusr2\n";
    write(1,"P1 received sigusr2\n",strlen(c));
    fflush(stdout);
}
int main()
{
    signal_handler_connection();
    // wait for p4 to send a signal
    // receive signal from p4 to determine left=p4
    receive_msg_msgq();
    // send a signal to p2
    kill(right, SIGUSR1);
    sleep(5);
    int self_pid = getpid();
    printf("P1 left %d, right %d, self %d\n", left, right, self_pid);

    // sigaction for circular
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = &circular_signal;
    if (sigaction(SIGUSR1, &act, NULL) == -1)
    {
        printf("Sigaction p1-2\n");
        exit(1);
    }

    act.sa_flags = 0;
    act.sa_handler = &reverse_circular_signal;
    if (sigaction(SIGUSR2, &act, NULL) == -1)
    {
        printf("Sigaction p1-2\n");
        exit(1);
    }
    sleep(1);

    // start circular signal
    for (int i = 0; i < 3; i++)
    {
        printf("P1 started sigusr1\n");
        kill(right, SIGUSR1);
        sleep(1);
    }

    // start reverse circular signal
    for (int i = 0; i < 3; i++)
    {
        printf("P1 started sigusr2\n");
        kill(left, SIGUSR2);
        sleep(1);
    }
}