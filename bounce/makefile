CFLAGS = -std=c++11 -Wall -pedantic
CC = g++

all: bin/bounce

bin/bounce: bounce.h bounce2d.cpp set_ticker.c
	mkdir -p bin
	$(CC) -g -o bin/bounce2d bounce2d.cpp set_ticker.c $(CFLAGS) -lcurses

.PHONY: clean
clean:
	mkdir -p bin
	-rm bin/*