#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{
    char buff[100];
    scanf("%s", buff);
    fflush(stdin);
    printf("P4 : Read %s from P3\n", buff);
    fflush(stdout);
}