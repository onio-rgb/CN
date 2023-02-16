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
    int sem_id=semget(key_sem,2,0666);

    int key_shm=ftok("/shm/s",222);
    int shm_id=shmget(key_shm,4096,0666);

    printf("I am waiting for s1\n");
    struct sembuf* sem_ops=malloc(sizeof(struct sembuf));
    sem_ops->sem_num=0;
    sem_ops->sem_op=-1;
    semop(sem_id,sem_ops,1);// wait s1



    struct shm_mem* mem=shmat(shm_id,NULL,0);
    printf("p1 : I am reading shm x\n");
    mem->y=mem->x+1;
    printf("x : %d y : %d \n",mem->x,mem->y);
    printf("Enter any char to signal S2\n");
    char ch;
    scanf("%c",&ch);

    sem_ops->sem_op=1;
    sem_ops->sem_num=1;
    semop(sem_id,sem_ops,1);// signal s2
    
}