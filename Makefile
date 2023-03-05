CC = gcc
CFLAGS = -Wall -g -std=c11

build: tema3.o
	$(CC) -o tema3 $^

tema3.o: tema3.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o tema3

.PHONY: clean build
