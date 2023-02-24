#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>
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
int msgid;
void signal_handler_connection()
{
    // receive signal by p1 (sigusr1)(left)
    // find out who sent the signal
    // find the pid of the sigusr1
    struct sigaction act;
    act.sa_flags = SA_SIGINFO;
    
    act.sa_sigaction = &handler;
    if (sigaction(SIGUSR1, &act, NULL) == -1)
    {
        printf("Sigaction p2\n");
        exit(1);
    }
}
void send_pid_msgq(int _pid, int type)
{
    // send its pid to p1 through msgq
    key_t key = ftok("/tmp/Q", 100);
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        printf("message queue not created\n");
        exit(1);
    }
    struct msgbuf _msgToP1;
    _msgToP1.mtype = type;
    int pid = _pid;
    char *pid_string;
    sprintf(_msgToP1.mtext, "p2%d", pid);
    int rvalue = msgsnd(msgid, &_msgToP1, sizeof(_msgToP1.mtext), 0);
}
void receive_msg_msgq_right()
{
    struct msgbuf msgp;
    msgrcv(msgid, &msgp, 200, 3, 0);
    if (msgp.mtype == 3)
    {
        char *pid_string = malloc(10);
        strncpy(pid_string, &msgp.mtext[2], strlen(msgp.mtext) - 2);
        int pid_int = atoi(pid_string);
        right = pid_int;
    }
}
void circular_signal(int sig)
{
    char *c="P2->P3 sigusr1\n";
    write(1,c,strlen(c));
    fflush(stdout);
    kill(right, SIGUSR1);
}
void reverse_circular_signal(int sig)
{
    char *c="P2->P1 sigusr2\n";
    write(1,c,strlen(c));
    fflush(stdout);
    kill(left, SIGUSR2);
}
int main()
{
    key_t key = ftok("/tmp/Q", 100);
    msgid = msgget(key, 0666 | IPC_CREAT);
    msgctl(msgid, IPC_RMID, NULL);
    int self_pid = getpid();
    signal_handler_connection();
    send_pid_msgq(self_pid, 2);
    int c = fork();
    if (c > 0)
    {
        int p3 = fork();
        if (p3 > 0)
        {
            // p2 receives type=3 msg from queue to find right=p3
            // if anything goeas wrong sleep for 1 sec here
            sleep(1);
            receive_msg_msgq_right();
            // p2 sends p1's pid to msgq
            send_pid_msgq(c, 1);
            // send sigusr1 to p3
            kill(right, SIGUSR1);
            printf("P2 left %d, right %d, self %d\n", left, right, self_pid);
        }
        else
        {
            // run p3
            char *args[1] = {NULL};
            execvp("./p3", args);
        }
    }
    else
    {
        // run p1
        char *args[1] = {NULL};
        execvp("./p1", args);
    }

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
