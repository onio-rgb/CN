#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>
#include <sys/ipc.h>
#include<sys/sem.h>
void waitSem(int sem_id)
{
    struct sembuf* sops=malloc(sizeof(struct sembuf));
    sops->sem_num=0;
    sops->sem_op=-1;
    semop(sem_id,sops,1);
}
void signalSem(int sem_id)
{
    struct sembuf* sops=malloc(sizeof(struct sembuf));
    sops->sem_num=1;
    sops->sem_op=1;
    semop(sem_id,sops,1);
}
void destroySem(int sem_id)
{
    semctl(sem_id,0,IPC_RMID);
}
int main()
{
    int p2c[2];
    int c2p[2];
    key_t key=ftok("/tmp/sem/t",100);
    int sem_id=semget(key,2,0666);
    destroySem(sem_id);
    key=ftok("/tmp/sem/t",100);
    sem_id=semget(key,2,0666 | IPC_CREAT);
    int stdincpy=dup(0);
    int stdoutcpy=dup(1);
    if(pipe(p2c)==-1){
        printf("Pipe p2c\n");
        return 1;
    }
    if(pipe(c2p)==-1)
    {
        printf("Pipe c2p\n");
        return 2;
    }
    int c=fork();
    if(c>0)
    {
        // sleep(1);
        close(p2c[0]);
        close(c2p[1]);
        char rd[20];
        int _readPipe=c2p[0];
        int _writePipe=p2c[1];
        while(1)
        {
            // input reditection to keyboard
            dup2(stdincpy,0);
            printf("Enter P1: \n");
            fgets(rd,20,stdin); //read from keyboard
            // output redirection to pipe
            dup2(_writePipe,1);
            write(1,rd,sizeof(rd)); // send to p2
            fflush(stdout);
            signalSem(sem_id);
            waitSem(sem_id);
            // input reditrection to pipe
            dup2(_readPipe,0);
            read(0,rd,20); // read from p2
            if(strlen(rd)==1 && rd[0]=='\n')
            {
                printf("P1 exiting null detected from p2\n");
                break;
            }
            // output redirection to screen 
            dup2(stdoutcpy,1);
            printf("P1 : %s\n",rd);
            fflush(stdout);

        }
        dup2(stdoutcpy,1);
        printf("p1 exited\n");
        fflush(stdout);
    }
    else {
        close(p2c[1]);
        close(c2p[0]);
        dup2(p2c[0],0);
        dup2(c2p[1],1);
        char in_char[5];
        sprintf(in_char,"%d",stdincpy);
        char out_char[5];
        sprintf(out_char,"%d",stdoutcpy);
        // printf("%s %s\n",in_char,out_char);
        char *argv[3]={in_char,out_char,NULL};
        execvp("./p2",argv);
        write(stdoutcpy,"fail execv\n",sizeof("fail execv\n"));
        return 3;
    }
}