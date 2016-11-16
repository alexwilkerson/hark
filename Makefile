# Makefile for building hark on Unix systems

all: clean hark

hark: hark.o
	clang -L/usr/local/lib -lSDL2 hark.o -o hark

hark.o:
	clang -c -Wall -Wextra -Wpedantic -g -std=gnu11 -I/usr/local/include hark.c -o hark.o

run:
	./hark

clean:
	-rm hark *.o
