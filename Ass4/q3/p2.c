#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
int main()
{
    // creating the semaphiore set for p2  s23
    // getting the semaphore set for p1 for s12
    key_t key12 = ftok("./sem/s1", 111);
    int sem_id12 = semget(key12, 2, 0666);
    printf("I am P2. I am waiting for Semaphore S12\n");
    // wait on s12
    struct sembuf *_semop = malloc(sizeof(struct sembuf));
    _semop->sem_num = 0;
    _semop->sem_op = -1;
    semop(sem_id12, _semop, 1);
    printf(" I got semaphore S12 signalling from P1\n");

    // creating s23
    key_t key23 = ftok("./sem/s23", 23);
    int sem_id23 = semget(key23, 1, 0666 | IPC_CREAT);

    _semop = malloc(sizeof(struct sembuf));
    _semop->sem_num = 0;
    _semop->sem_op = 1;
    // signal s23
    printf(" Enter any character to sem-signal( S23)\n");
    char c;
    scanf("%c", &c);
    printf(" I am signalling semaphore signal of S23\n");
    semop(sem_id23, _semop, 1);
}