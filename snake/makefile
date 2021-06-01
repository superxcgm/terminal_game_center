CC = gcc

all: snake

snake: linklist.o set_ticker.o snake.o res.o xc_queue.o
	$(CC) -o snake -g linklist.o set_ticker.o snake.o res.o xc_queue.o -lcurses

snake.o: snake.c snake.h
	$(CC) -c -g snake.c

set_ticker.o: set_ticker.c
	$(CC) -c -g set_ticker.c

linklist.o: linklist.c linklist.h
	$(CC) -c -g linklist.c

res.o: res.c res.h
	$(CC) -c -g res.c

xc_queue.o: xc_queue.c xc_queue.h
	$(CC) -c -g xc_queue.c

.PHONY: clean
clean:
	rm snake snake.o set_ticker.o linklist.o res.o xc_queue.o