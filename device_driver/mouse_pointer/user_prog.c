#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include<errno.h>
int errno;
struct coord
{
    int step_count; // make sure it is a common factor of rel_x and rel_y (testing only) 
    int rel_x;
    int rel_y;
};
int main()
{
    char *driver_file_node_path = "/dev/mouse_point0";
    int fd = open(driver_file_node_path, O_RDWR);
    perror("");
    struct coord *move_by = malloc(sizeof(struct coord));
    move_by->step_count=100;
    move_by->rel_x = 1100;
    move_by->rel_y = 1200;
    write(fd, (char *)move_by, sizeof(struct coord));
    char *return_msg = malloc(200);
    read(fd, return_msg, 200);
    printf("Driver returned %s %d \n", return_msg,fd);
    close(fd);
}