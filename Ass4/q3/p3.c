#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
int main()
{
    // get s23
    key_t key23 = ftok("./sem/s23", 23);
    int sem_id23 = semget(key23, 1, 0666);
    printf("I am P3. I am waiting for Semaphore S23\n");
    // wait on s23
    struct sembuf *_semop = malloc(sizeof(struct sembuf));
    _semop->sem_num = 0;
    _semop->sem_op = -1;
    semop(sem_id23, _semop, 1);
    printf(" I got semaphore S23 signalling from P2\n");

    // creating s34
    key_t key34 = ftok("./sem/s234", 34);
    int sem_id34 = semget(key34, 1, 0666 | IPC_CREAT);

    _semop = malloc(sizeof(struct sembuf));
    _semop->sem_num = 0;
    _semop->sem_op = 1;
    // signal s34
    printf(" Enter any character to sem-signal( S34)\n");
    char c;
    scanf("%c", &c);
    printf(" I am signalling semaphore signal of S34\n");
    semop(sem_id34, _semop, 1);
}