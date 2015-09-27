# COP4610 Project 1 Makefile
CC = gcc

all: shell.x

shell.x: shell_util.o
	$(CC) shell.c shell_util.c -o shell.x
  
clean:
	rm *o shell.x
