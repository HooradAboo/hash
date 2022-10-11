#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

char buf[512];
char lines[100][100];

int sort(int fd, int mode, int fd_out) 
{   
    // FILE *fd;
    // int fd_out = -1;
    // int command = 0;
    int r_flag = 0, o_flag = 0, n_flag = 0;

    int n;
    int i, j = 0, k = 0;
    int row = 0;

    // printf("%s ", args[0]);
    // printf("%s ", args[1]);
    // printf("%s", args[2]);
    // char* input[10] = {"sort", "test.txt"};
    // args[0] = "sort";
    // args[1] = "test.txt";

    // for(i = 1; args[i] != NULL; i++) {
    //     // printf("%p\n", args[i]);
    //     // printf("%p", input[i]);
    //     if(strcmp(args[i], "-r") == 0) {
    //         // printf("1\n");
    //         r_flag = 1;

    //     } else if(strcmp(args[i], "-o") == 0) {
    //         // printf("2\n");
    //         o_flag = 1;
    //         i++;
    //         fd_out = open(args[i], O_CREAT | O_RDWR);

    //     } else if(strcmp(args[i], "-n") == 0) {
    //         // printf("3\n");
    //         n_flag = 1;

    //     } else if((fd = fopen(args[i], 0)) < 0) {
    //         printf("sort: cannot open %s\n", args[i]);
    //         return 1;
    //     }
    //     // printf("%s", args[i]);
    // }

    switch (mode)
    {
    case 1:
        r_flag = 1;
        break;
    case 2:
        o_flag = 1;
        break;
    case 3:
        n_flag = 1;
        break;
    }

    // fd = fopen("test.txt", 0);
    // printf("kir");
    // printf("%s", args[i]);
    // printf("%d", fd);
    // read(fd, buf, sizeof(buf));
    // printf("%d", n);
    // printf("%d", fd);
    // fgets(buf, 50, fd);

    // get each line in lines array
    for(i = 0; i < n; i++) {
        lines[row][j] = buf[i];
        j++;
        if(buf[i] == '\n' || buf[i] == EOF) {
            row++;
            j = 0;
        }
    }

    int len = row - 1;
    char tmp[100];
    // bubble sort ascending
    for(j = 0; j <= len; j++) {
        for(i = 0; i <= len - 1; i++) {
            if(strcmp(lines[i], lines[i+1]) > 0) {
                strcpy(tmp, lines[i]);  
                strcpy(lines[i], lines[i+1]);
                strcpy(lines[i+1], tmp);        
            }
        }
    }

    if (r_flag == 1) {
        // bubble sort descending
        for(j = 0; j <= len; j++) {
            for(i = 0; i <= len - 1; i++) {
                if(strcmp(lines[i], lines[i+1]) < 0) {
                    strcpy(tmp, lines[i]);  
                    strcpy(lines[i], lines[i+1]);
                    strcpy(lines[i+1], tmp);        
                }
            }
        }
    }

    for(i = 0; i < row; i++) {
        for(j = 0; j < 100; j++) {
            if (o_flag == 1) {
                buf[k++] = lines[i][j];
            }
            printf("%c", lines[i][j]);
            if(lines[i][j] == '\n') {
                break;
            } 
        }
    }

    if (o_flag == 1) {
        write(fd_out, buf, sizeof(buf));
    }

    fclose(fd);
    
    return 1; 
} 


int main(int argc, char *argv[])
{
    int fd, fd_out;
    int i;

    int mode = 0;

    for(i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-r") == 0) {
            mode = 1;
        } else if(strcmp(argv[i], "-o") == 0) {
            mode = 2;
            i++;
            fd_out = open(argv[i], O_CREAT | O_RDWR);
        } else if(strcmp(argv[i], "-n") == 0) {
            mode = 3;
        } else if((fd = open(argv[i], 0)) > 0) {
            sort(fd, mode, fd_out);
            close(fd);
        }
    }

    exit(0);
}