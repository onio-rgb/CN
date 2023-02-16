#include<stdio.h>
#include<sys/sem.h>
#include<stdlib.h>
#include<unistd.h>
#include  <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
struct shm_mem{
    int x;
    int y;
};
int main()
{
    int key_sem=ftok("/sem/s",111);
    int sem_id=semget(key_sem,2,0666|IPC_CREAT);

    int key_shm=ftok("/shm/s",222);
    int shm_id=shmget(key_shm,4096,0666|IPC_CREAT);

    struct shm_mem* mem=shmat(shm_id,NULL,0);
    mem->x=1;
    mem->y=1;
    printf("I am reading shm y\n");
    mem->x=mem->y+1;
    printf("x : %d y : %d \n",mem->x,mem->y);
    printf("Enter any char to signal S1\n");
    char ch;
    scanf("%c",&ch);
    
    struct sembuf* sem_ops=malloc(sizeof(struct sembuf));
    sem_ops->sem_num=0;
    sem_ops->sem_op=1;
    semop(sem_id,sem_ops,1);// singal s1

    printf("I am waiting for s2\n");
    sem_ops->sem_op=-1;
    sem_ops->sem_num=1;
    semop(sem_id,sem_ops,1);// wait s2

}