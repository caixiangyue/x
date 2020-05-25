CC=gcc
CFLAGS=-g -Wall -Wextra -pedantic  -std=c99

x:x.c
	$(CC) x.c -o x $(CFLAGS)
clean:
	rm x
