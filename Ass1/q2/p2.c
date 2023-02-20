#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>
#include <sys/ipc.h>
#include<sys/sem.h>
void waitSem(int sem_id)
{
    struct sembuf* sops=malloc(sizeof(struct sembuf));
    sops->sem_num=1;
    sops->sem_op=-1;
    semop(sem_id,sops,1);
}
void signalSem(int sem_id)
{
    struct sembuf* sops=malloc(sizeof(struct sembuf));
    sops->sem_num=0;
    sops->sem_op=1;
    semop(sem_id,sops,1);
}
void destroySem(int sem_id)
{
    semctl(sem_id,0,IPC_RMID);
}
int main(int argc,char * argv[])
{
    key_t key=ftok("/tmp/sem/t",100);
    int sem_id=semget(key,2,0666);
    int _readPipe=dup(0);
    int _writePipe=dup(1);
    char s[20];
    int stdincpy=atoi(argv[0]);
    int stdoutcpy=atoi(argv[1]);
    dup2(stdoutcpy,1);
    write(1,"This is p2\n",sizeof("This is p2\n"));
    fflush(stdout);
    dup2(_writePipe,1);

    while(1)
    {
        waitSem(sem_id);
        // input reditection to pipe
        dup2(_readPipe,0);
        // output redirection to screen
        dup2(stdoutcpy,1);
        read(0,s,20); // read from pipe
        if(strlen(s)==1 && s[0]=='\n'){
            printf("P2 exiting null detected from p1\n");
            break;
        }
        printf("P2 : %s\n",s);

        // input redirection to keyboard
        dup2(stdincpy,0);
        printf("Enter P2: \n");
        fgets(s,20,stdin); 

        //output redirection to pipe c2p
        dup2(_writePipe,1);
        write(1,s,sizeof(s));
        fflush(stdout);
        signalSem(sem_id);
    }

}