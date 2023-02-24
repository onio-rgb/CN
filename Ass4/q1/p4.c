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
        printf("Sigaction p4\n");
        exit(1);
    }
}
int msgid;
void send_pid_msgq()
{
    key_t key = ftok("/tmp/Q", 100);
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        printf("message queue not created\n");
        exit(1);
    }
    struct msgbuf _msgToP2;
    _msgToP2.mtype = 4;
    int pid = getpid();
    char *pid_string;
    sprintf(_msgToP2.mtext, "p4%d", pid);
    int rvalue = msgsnd(msgid, &_msgToP2, sizeof(_msgToP2.mtext), 0);
}
void receive_msg_msgq()
{
    struct msgbuf msgp;
    msgrcv(msgid, &msgp, 200, 1, 0);
    if (msgp.mtype == 1)
    {
        char *pid_string = malloc(10);
        strncpy(pid_string, &msgp.mtext[2], strlen(msgp.mtext) - 2);
        int pid_int = atoi(pid_string);
        right = pid_int;
    }
}
void circular_signal(int sig)
{
    char *c="P4->P1 sigusr1\n";
    write(1,c,strlen(c));
    fflush(stdout);
    kill(right, SIGUSR1);
}
void reverse_circular_signal(int sig)
{
    char *c="P4->P3 sigusr2\n";
    write(1,c,strlen(c));
    fflush(stdout);
    kill(left, SIGUSR2);
}
int main()
{
    // P4 catches the signal and it finds out, who has sent that signal to it
    signal_handler_connection();
    // send its pid to p3 through msgq
    send_pid_msgq();
    receive_msg_msgq();
    // sends a SIGUSR1 to P1
    kill(right, SIGUSR1);
    int self_pid = getpid();
    sleep(1);
    printf("P4 left %d, right %d, self %d\n", left, right, self_pid);
    // sigaction for circular signalling
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
    
    for(int i=0;i<3;i++)sleep(10);
    for(int i=0;i<3;i++)sleep(10);
}