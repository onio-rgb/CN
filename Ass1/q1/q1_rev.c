#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main()
{
    int c=fork();
    if(c>0)
    {
        char buff[100];
        scanf("%s",buff);
        
    }
}