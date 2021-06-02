CC = g++
CFLAGS = -std=c++17 -Wall -pedantic

all: snake

snake: set_ticker.o snake.o res.o
	$(CC) -o snake -g set_ticker.o snake.o res.o $(CFLAGS) -lcurses

snake.o: snake.cpp snake.h
	$(CC) -c -g snake.cpp $(CFLAGS)

set_ticker.o: set_ticker.c
	$(CC) -c -g set_ticker.c $(CFLAGS)

res.o: res.cpp res.h
	$(CC) -c -g res.cpp $(CFLAGS)

.PHONY: clean
clean:
	-rm snake *.o