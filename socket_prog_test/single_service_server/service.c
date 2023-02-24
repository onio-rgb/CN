#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
    char *buff = malloc(200);
    recv(0, buff, 200, 0);
    send(1, "File directory recieved \n", sizeof("File directory recieved \n"), 0);
    char *command = malloc(200);
    sprintf(command, "cat %s", buff);
    FILE *fp = popen(command, "r");
    char *line = malloc(1024);
    while (fgets(line, 1024, fp) != NULL)
    {
        printf("%s", line);
    }
}