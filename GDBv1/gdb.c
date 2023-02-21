#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#define FIFO "/home/onio/read_cpp"
int check()
{
    FILE *correct_out=malloc(sizeof(FILE));
    correct_out = fopen("./OT.txt", "r");
    FILE *user_out=malloc(sizeof(FILE));
    user_out = fopen("./Pout.txt", "r");
    size_t l1=0;
    char * s1=0;
    size_t l2=0;
    char * s2=0;
    while(getline(&s1,&l1,user_out)>0 && getline(&s2,&l2,correct_out)>0)
    {
        printf("%s",s1);
        if(strcmp(s1,s2)!=0)
        {
            return 0;
        }
    }
    return 1;
}
int main()
{
    unlink(FIFO);
    mkfifo(FIFO, 0666);
    int fd = open(FIFO, O_RDWR);
    system("touch P.cpp");
    
        char *buff = malloc(sizeof(char) * 1024);
        int n_read = read(fd, buff, 1024);
        if (n_read > 0)
        {
            int cpp_fd = open("./P.cpp", O_WRONLY);
            write(cpp_fd, buff, n_read);
            system("g++ P.cpp -o P.exe");
            // system("./P.exe > Pout.text < IN.txt");
            int c = fork();
            if (c > 0)
            {
                wait(NULL);
                if(check())
                {
                    char *msg="Correct Output!";
                    write(fd,msg,strlen(msg));
                }
                else {
                    char *msg="Wrong Output!";
                    write(fd,msg,strlen(msg));
                }
            }
            else
            {
                int infd = open("IT.txt", O_RDONLY);
                int outfd = open("Pout.txt",O_WRONLY | O_CREAT);
                printf("executing \n");
                fflush(stdout);
                int stdout_cpy=dup(1);
                int stdin_cpy=dup(0);
                dup2(infd, 0);
                dup2(outfd, 1);
                char *const dumm[1]={NULL};
                execvp("./P.exe", dumm);
            }
        }
        else if (n_read == -1)
        {
            printf("Error \n");
        }
        else
        {
            printf("Np file inputted\n");
        }
    
    close(fd);
}