#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
int main()
{
    char *buff = malloc(300);
    scanf("%[^\n]", buff);
    
    printf("%s\n", buff);
    fflush(stdout);
}