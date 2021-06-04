CC = g++
CFLAGS = -std=c++17 -Wall -pedantic

all: snake

snake: set_ticker.o snake.o res.o game.o main.cpp
	$(CC) -o snake -g set_ticker.o snake.o res.o game.o main.cpp $(CFLAGS) -lcurses

snake.o: snake.cpp snake.h
	$(CC) -c -g snake.cpp $(CFLAGS)

set_ticker.o: set_ticker.c
	$(CC) -c -g set_ticker.c $(CFLAGS)

res.o: res.cpp res.h
	$(CC) -c -g res.cpp $(CFLAGS)

game.o: game.cpp game.h
	$(CC) -c -g game.cpp $(CFLAGS)

.PHONY: clean
clean:
	-rm snake *.o

.PHONY: debug

debug:
	./snake 2> log.txt
