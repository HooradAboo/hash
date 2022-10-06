#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFF_SIZE 512
#define TOK_SIZE 128
#define TOK_DELIM " \t\r\n\a"

#define APPEND 1
#define OUTPUT 2
#define INPUT 3

#define WHITE   "\x1B[37m"
#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"
#define BLUE    "\x1b[34m"


char* redirect (char* line);


char* username () {
    return getenv("USER");
}


void parse (char *line, char** args, int *argc, const char* delim) {
    // char* line_copy = malloc(sizeof(line));
    // strcpy(line_copy, line);
    char* arg;

	arg = strtok(line, delim);
	int pc = -1;

	while(arg) {
		args[++pc] = malloc(sizeof(arg)+1);
		strcpy(args[pc], arg);

        // Remove spaces
        if(args[pc][0] == ' ' || args[pc][0] == '\n') {
            memmove(args[pc], args[pc]+1, strlen(args[pc]));
        }
        if(args[pc][strlen(args[pc])-1] == ' ' || args[pc][strlen(args[pc])-1] == '\n') {
            args[pc][strlen(args[pc])-1] = '\0';
        }

		arg = strtok(NULL,delim);
	}
	args[++pc] = NULL;
	*argc = pc;
}


void execute (char *line) {
    char* line_copy = malloc(sizeof(line));
    strcpy(line_copy, line);

    char *args[BUFF_SIZE];
    int argc;
    char *tmp;    

    pid_t pid = fork();
    
    if (pid == 0) {
        tmp = redirect(line);
        parse(tmp, args, &argc, " ");
        if (execvp(args[0], args) == -1) {
            perror(RED "Invalid input");
        } 
    } else if (pid == -1) {
        perror(RED "Invalid input");
        return;

    } else {
        wait(NULL);
    }
} 


void execute_pipe (char* line) {
    char* pipes[BUFF_SIZE];
    int pipec;
    parse (line, pipes, &pipec, "|");

    char* args[BUFF_SIZE];
    int argc;
    int i;

    pid_t pid;

    // fd[0] will be the fd (file descriptor) for the read end of pipe 
    // and fd[1] will be the fd for the write end of pipe.
    int fd[pipec][2];  

    int rdir_input;
    int rdir_output;
    int rdir_append;

    char *tmp;    

    
    for (i = 0; i < pipec; i++) {           
        // parse(pipes[i], args, &argc, " \n");

        if (strchr(pipes[i], '<')) rdir_input = 1;
        if (strchr(pipes[i], '>')) rdir_output = 1;
        // if (strchr(pipes[i], '>>')) rdir_append = 1;

        if(i != pipec - 1) {          // Not last pipe
			if(pipe(fd[i]) < 0) {
				perror(RED "Pipe creating was not successfull\n");
				return;
			}
		}
        
        pid = fork();
        tmp = redirect(pipes[i]);
        parse(tmp, args, &argc, " \n");

        if (pid == 0) { // Chile process
            // tmp = redirect(pipes[i]);
            // parse(tmp, args, &argc, " \n");
            if (i == 0) {                       // Firs pipe - no input
                if (rdir_output == 0 && rdir_append == 0)
                    dup2(fd[i][1], 1);
				close(fd[i][0]);
				close(fd[i][1]);

            } else if (i == pipec - 1) {        // Last pipe - no output
                if (rdir_input == 0)
                    dup2(fd[i-1][0], 0);
				close(fd[i-1][1]);
				close(fd[i-1][0]);

            } else {                            // Middle pipes
                if (rdir_input == 0)
                    dup2(fd[i-1][0], 0);
				close(fd[i-1][1]);
				close(fd[i-1][0]);
                if (rdir_output == 0 && rdir_append == 0)
                    dup2(fd[i][1], 1);
				close(fd[i][0]);
				close(fd[i][1]);
            }
            
            if (execvp(args[0], args) == -1) {
                perror(RED "Invalid input"); 
                return;
            } 
                      
            
        } else if (pid == -1) {
            perror(RED "Invalid input");
            return;
        }
    }
    close(fd[i-1][0]);
    close(fd[i-1][1]);
    wait(NULL);
} 


char* redirect (char* line) {
    char* line_copy = malloc(sizeof(line));
    strcpy(line_copy, line);

    char *args[BUFF_SIZE];
    int argc;
    parse(line_copy, args, &argc, "><>>");

    int i;
    int pc = 0;
    char  *input, *output;
    int flags;

    int fdin, fdout;

    for (i = 0; i < strlen(line); i++) {
        if (line[i] == '>') {
            pc++;
            output = args[pc];
            if (line[i+1] == '>') {
                flags = O_WRONLY | O_APPEND | O_CREAT;
            } else {
                flags = O_WRONLY | O_CREAT;
            }
            fdout = open(output, flags, 0666);
            if (fdout < 0) {
                perror(RED "could not open output file");
                exit(1);
            }
            dup2(fdout, 1);
            close(fdout);
        }

        if (line[i] == '<') {
            pc++;
            input = args[pc];
            flags = O_RDONLY | O_CREAT;
            fdin = open(input, flags, 0666);
            if (fdin < 0) {
                perror(RED "could not open input file");
                exit(1);
            }
            dup2(fdin, 0);
            close(fdin);
        }
    }
    return args[0];
}


int main() {
    char dir[BUFF_SIZE];
    char host[BUFF_SIZE];
    char line[500];
    char* args[BUFF_SIZE];
    int argc = 0;
    int i;
    char tmp[100];

    while(1) {
        gethostname(host, sizeof(host));
        getcwd(dir, BUFF_SIZE);
        printf(GREEN "%s@%s" WHITE ":" BLUE "%s %d$ " WHITE, username(), host, dir, getpid());

        // read user input
        fgets(line, 500, stdin);

        if (strchr(line, '|')) {                 // pipe commands
            execute_pipe(line);

        } else if (strchr(line, '>')) {                 // redirect output to file
            execute(line);

        } else if (strchr(line, '<')) {                 // redirect output to file
            execute(line);

        } else {                            // single command
            execute(line);  
        }
        sleep(1);
    }
}