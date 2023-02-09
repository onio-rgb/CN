#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
int main()
{
    // get s34
    key_t key34 = ftok("./sem/s34", 34);
    int sem_id34 = semget(key34, 1, 0666);
    printf("I am P4. I am waiting for Semaphore S34\n");
    // wait on s34
    struct sembuf *_semop = malloc(sizeof(struct sembuf));
    _semop->sem_num = 0;
    _semop->sem_op = -1;
    semop(sem_id34, _semop, 1);
    printf(" I got semaphore S34 signalling from P3\n");

    printf(" Enter any character to sem-signal( S41)\n");
    char c;
    scanf("%c", &c);
    printf(" I am signalling semaphore signal of S41\n");
    // get s41
    key_t key41 = ftok("./sem/s1", 111);
    int sem_id41 = semget(key41, 2, 0666);

    _semop = malloc(sizeof(struct sembuf));
    _semop->sem_num = 1;
    _semop->sem_op = 1;
    // signal s41
    semop(sem_id41, _semop, 1);
}