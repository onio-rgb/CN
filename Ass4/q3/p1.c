#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
int main()
{
    key_t key=ftok("./sem/s1",112);
    int sem_id=semget(key,2,0666 | IPC_CREAT);
    
}