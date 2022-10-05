# source files.
SRC =  hash.c
OBJ = $(SRC:.c=.o)

OUT = hash.o

# include directories
INCLUDES = -I. -I/usr/local/include -Iutility/

# compiler
CC = gcc

$(OUT): $(SRC)
	$(CC) $(INCLUDES) -o $(OUT) $(SRC) 