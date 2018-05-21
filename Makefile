CC=gcc
CFLAGS=-O2 -Wall

main: main.o sshbrute.o
	$(CC) $(CFLAGS) -o $@ $^ -lssh

main.o: main.c
	$(CC) $(CFLAGS) -c $<

sshbrute.o: sshbrute.c sshbrute.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o main

