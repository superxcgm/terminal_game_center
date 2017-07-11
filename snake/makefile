snake : linklist.o set_ticker.o snake.o res.o xc_queue.o
	gcc -lcurses -o snake linklist.o set_ticker.o snake.o res.o xc_queue.o

snake.o : snake.c snake.h
	gcc -c snake.c

set_ticker.o : set_ticker.c
	gcc -c set_ticker.c

linklist.o : linklist.c linklist.h
	gcc -c linklist.c

res.o : res.c res.h
	gcc -c res.c

xc_queue.o : xc_queue.c xc_queue.h
	gcc -c xc_queue.c

clean :
	rm snake snake.o set_ticker.o linklist.o res.o xc_queue.o