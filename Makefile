CC = gcc
CFLAGS = -Wall -std=c99

all: bellman

bellman.o: bellman.c 
	$(CC) $(CFLAGS) -c bellman.c
	
bellman: bellman.o
	$(CC) $(CFLAGS) -o bellman bellman.o

clean:
	rm *.o bellman
