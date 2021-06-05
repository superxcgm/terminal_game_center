CC = g++
CFLAGS = -std=c++17 -Wall -pedantic

all: snake

snake: set_ticker.o snake.o res.o game.o main.cpp graph.o menu.o log.o
	$(CC) -o snake -g set_ticker.o snake.o res.o game.o graph.o menu.o log.o main.cpp $(CFLAGS) -lcurses

snake.o: snake.cpp snake.h
	$(CC) -c -g snake.cpp $(CFLAGS)

set_ticker.o: set_ticker.c
	$(CC) -c -g set_ticker.c $(CFLAGS)

res.o: res.cpp res.h
	$(CC) -c -g res.cpp $(CFLAGS)

game.o: game.cpp game.h
	$(CC) -c -g game.cpp $(CFLAGS)

graph.o: graph.h graph.cpp
	$(CC) -c -g graph.cpp $(CFLAGS)

menu.o: menu.cpp menu.h
	$(CC) -c -g menu.cpp $(CFLAGS)

log.o: log.cpp log.h
	$(CC) -c -g log.cpp $(CFLAGS)

.PHONY: clean
clean:
	-rm snake *.o
