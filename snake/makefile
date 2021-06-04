CC = g++
CFLAGS = -std=c++17 -Wall -pedantic

all: snake

snake: config.h game.cpp game.h graph.h main.cpp res.cpp res.h set_ticker.c snake.cpp snake.h
	$(CC) -o snake -g game.cpp main.cpp res.cpp snake.cpp set_ticker.c $(CFLAGS) -lcurses

.PHONY: clean
clean:
	-rm snake

.PHONY: debug

debug:
	./snake 2> log.txt
