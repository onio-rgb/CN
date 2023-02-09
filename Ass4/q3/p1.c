#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
int main()
{
    // creating the semaphiore set for p1 s12 and s41
    key_t key = ftok("./sem/s1", 111);
    int sem_id = semget(key, 2, 0666 | IPC_CREAT);
    printf("Enter any char to sem-signal s12\n");
    char c;
    scanf("%c", &c);
    struct sembuf *_semop = malloc(sizeof(struct sembuf));
    _semop->sem_num = 0;
    _semop->sem_op = 1;
    printf(" I am signalling semaphore signal of S12\n");
    semop(sem_id, _semop, 1);

    _semop = malloc(sizeof(struct sembuf));
    _semop->sem_num = 1;
    _semop->sem_op = -1;
    // wait on s41
    printf("I am waiting for semaphore S41\n");
    semop(sem_id, _semop, 1);
    printf(" I got semaphore signalling from P4 \n");
}