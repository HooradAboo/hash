#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "core.h"

#define BUFF_SIZE 128
#define TOK_SIZE 32
#define TOK_DELIM " \t\r\n\a"


char* readl ()
{
    char *buff = NULL;
    size_t bufsize = 0;

    getline(&buff, &bufsize, stdin);

    return buff; 
}


char** parse (char* line)
{
    int bufsize = TOK_SIZE;
    char** args = malloc(bufsize * sizeof(char*));
    char* arg;
    int i = 0;

    arg = strtok(line, TOK_DELIM);
    while (arg != NULL)
    {
        args[i++] = arg;
        arg = strtok(NULL, TOK_DELIM);
    }
    args[i] = NULL;

    return args;
}


int execute (char** args)
{
    int i;

    if (args[0] == NULL) {
        return 1;
    }

    for (i = 0; i < num_funcitons (); i++) {
        // printf("%d\n", strcmp(args[0], func_name[i]));
        if(strcmp(args[0], func_name[i]) == 0) {
            // printf("%s\n", args[0]);
            return (*functions[i])(args);
        }
    }

    return 1;
}


int main () 
{
    char* line;
    char** args;
    int status;
    int i;

    char* username = getenv("USER");
    char dir[512]; 

    while (status) {
        getcwd(dir, sizeof(dir));
        printf("%s:%s$ ", username, dir);
        line = readl();
        args = parse(line);
        status = execute(args);
    }
}


