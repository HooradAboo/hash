# source files.
SRC =  shell.c

OBJ = $(SRC:.c=.o)

OUT = shell.o

# include directories
INCLUDES = -I. -I/usr/local/include -Iutility/

# compiler
CC = gcc

$(OUT): $(SRC)
	$(CC) $(INCLUDES) -o $(OUT) $(SRC) 