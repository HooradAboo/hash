#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
 
char buf[2048], line[100], nxt_line[100];

void uniq(int fd, int command)
{ 
    char tmp_line[100], tmp_nxt_line[100];
    int len_line, len_nxt_line;
    int nxt_n, num_lines = 0;
    int i, j, k;
    int eof_line[512];
    int duplicate = 1;
    int counter = 1;
    char tmp[10];

    // Find the indexes of '\n'
    read(fd, buf, sizeof(buf));
    for(i = 0; i < sizeof(buf); i++) {
        if(buf[i] == '\n') {
            eof_line[num_lines++] = i;
        }
    }
    
    // get and print first line
    for(i = 0; i <= eof_line[0]; i++) {
        line[i] = buf[i];
    }
    len_line = eof_line[0]+1;

    // get and pritn non-duplicate lines.
    for(j = 1; j < num_lines; j++) {
        // get next line
        k = 0;
        duplicate = 0;
        for(i = eof_line[j-1]+1; i <= eof_line[j]; i++) {
            nxt_line[k++] = buf[i];
        }
        len_nxt_line = k;
        
        // if two lines hase same length, then check the similarity
        // if these are the same, set duplicate flag as 1
        if(len_line == len_nxt_line) {
            duplicate = 1;
            counter++;
            if(command == 2) {      // -i
                for(i = 0; i < len_line; i++) {
                    if(97 <= line[i] && line[i] <= 122){
                        tmp_line[i] = line[i] - 32;
                    } else {
                        tmp_line[i] = line[i];
                    }
                    if(97 <= nxt_line[i] && nxt_line[i] <= 122){
                        tmp_nxt_line[i] = nxt_line[i] - 32;
                    } else {
                        tmp_nxt_line[i] = nxt_line[i];
                    }
                    if(tmp_line[i] != tmp_nxt_line[i]) {
                        duplicate = 0;
                        break;
                    }
                }
            } else {
                for(i = 0; i < len_line; i++) {
                    if(line[i] != nxt_line[i]) {
                        counter--;
                        duplicate = 0;
                        break;
                    }
                }
            }  
        }

        // if the lines are not the same, print the next line
        // and assign nxt_line to line
        if(duplicate == 0) {
            if(command == 1) {          // -c
                printf("%d ", counter);
                counter = 1;
                // printf("%s", line);
                write(1, line, len_line);
                for(i = 0; i <= len_nxt_line; i++) {
                    line[i] = nxt_line[i];
                }
                len_line = len_nxt_line;
                if(j == num_lines - 1) {
                    printf("%d ", counter);
                    // printf("%s", line);
                    write(1, nxt_line, len_nxt_line);
                }
            } else if(command == 3) {
                if(counter > 1) {
                    write(1, line, len_line);
                    counter = 1;
                    // if(j == num_lines - 2) {
                    //     write(1, nxt_line, len_nxt_line);
                    // }
                } 
                for(i = 0; i <= len_nxt_line; i++) {
                    line[i] = nxt_line[i];
                }
                len_line = len_nxt_line;    
            } else {
                write(1, line, len_line);
                for(i = 0; i <= len_nxt_line; i++) {
                    line[i] = nxt_line[i];
                }
                len_line = len_nxt_line;
                if(j == num_lines - 1) {
                    write(1, nxt_line, len_nxt_line);
                }
            }
            
        }

        if(duplicate == 1) {
            if(counter > 1) {
                if(j == num_lines - 1) {
                    write(1, nxt_line, len_nxt_line);
                }
            }
        }
    }
} 


int main(int argc, char *argv[]) 
{ 
    int fd;
    int i;
    int cmd = 0;
    // argv = {"uniq", "test.txt"};

    // printf("my uniq");

    for(i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-c") == 0) {
            cmd = 1;
        } else if(strcmp(argv[i], "-i") == 0) {
            cmd = 2;
        } else if(strcmp(argv[i], "-d") == 0) {
            cmd = 3;
        } else if((fd = open(argv[i], 0)) > 0) {
            uniq(fd, cmd);
            close(fd);
        }
    }

    exit(0);
} 