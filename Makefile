# source files.
SRC =  hush.c
OBJ = $(SRC:.c=.o)

OUT = hush.o

# include directories
INCLUDES = -I. -I/usr/local/include -Iutility/

# compiler
CC = gcc

$(OUT): $(SRC)
	$(CC) $(INCLUDES) -o $(OUT) $(SRC) 