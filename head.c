#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
 
char buf[512];

void head(int fd, int n)
{
    int num_lines = 0;
    int i;

    read(fd, buf, sizeof(buf));

    for(i = 0; num_lines < n; i++) {
        if(buf[i] == '\n' || buf[i] == EOF) {
            num_lines++;
        }
    }

    write(1, buf, i);
}

int main(int argc, char *argv[]) 
{ 
    int fd;
    int i;
    int num = 10;

    for(i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            i++;
            num = atoi(argv[i]);
        } else if ((fd = open(argv[i], 0)) > 0) {
            head(fd, num);
            close(fd);
        }
    }

    exit(0);
} 
