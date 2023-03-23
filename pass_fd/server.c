#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/select.h>
#define ADDRESS "mysocket"
int main()
{
    int usfd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un userv_addr;
    userv_addr.sun_family = AF_UNIX;
    strcpy(userv_addr.sun_path, ADDRESS);
    bind(usfd, (struct sockaddr *)&userv_addr, sizeof(userv_addr));
    listen(usfd, 5);
    struct sockaddr client_addr;
    socklen_t client_len;
    int nusfd = accept(usfd, &client_addr, &client_len);
}