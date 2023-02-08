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
void signal_handler_connection()
{
    // receive signal by p2 (sigusr1)(left)
    // find out who sent the signal
    // find the pid of the sigusr1
    struct sigaction act;
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = &handler;
    if (sigaction(SIGUSR1, &act, NULL) == -1)
    {
        printf("Sigaction p3\n");
        exit(1);
    }
}
int msgid;
void send_pid_msgq(int _pid)
{
    key_t key = ftok("/tmp/Q", 100);
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        printf("message queue not created\n");
        exit(1);
    }
    struct msgbuf _msgToP2;
    _msgToP2.mtype = 3;
    int pid = _pid;
    char *pid_string;
    sprintf(_msgToP2.mtext, "p3%d", pid);
    int rvalue = msgsnd(msgid, &_msgToP2, sizeof(_msgToP2.mtext), 0);
}
void receive_msg_msgq_right()
{
    struct msgbuf msgp;
    msgrcv(msgid, &msgp, 200, 4, 0);
    if (msgp.mtype == 4)
    {
        char *pid_string = malloc(10);
        strncpy(pid_string, &msgp.mtext[2], strlen(msgp.mtext) - 2);
        int pid_int = atoi(pid_string);
        right = pid_int;
    }
}
void circular_signal(int sig)
{
    char *c="P3->P4 sigusr1\n";
    write(1,c,strlen(c));
    fflush(stdout);
    kill(right,SIGUSR1);
}
void reverse_circular_signal(int sig)
{
    char *c="P3->P2 sigusr2\n";
    write(1,c,strlen(c));
    fflush(stdout);
    kill(left, SIGUSR2);
}
int main()
{
    int self_pid=getpid();
    signal_handler_connection();
    // send its pid to p2 through msgq
    send_pid_msgq(self_pid);
    sleep(1);
    // fork p4
    int c = fork();
    if (c > 0)
    {
        sleep(1);
        // p3 receives type=4 msg from queue to find right=p4
        receive_msg_msgq_right();
        // P3 sends a signal SIGUSR1 to P4.
        
        kill(right, SIGUSR1);
        printf("P3 left %d, right %d, self %d\n", left, right, self_pid);
    }
    else
    {
        // run p4
        char *args[1] = {NULL};
        execvp("./p4", args);
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

    for(int i=0;i<3;i++)sleep(2);
    for(int i=0;i<3;i++)sleep(2);
}