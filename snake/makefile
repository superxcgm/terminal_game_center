snake : linklist.o set_ticker.o snake.o res.o xc_queue.o
	gcc -o snake -g linklist.o set_ticker.o snake.o res.o xc_queue.o -lcurses

snake.o : snake.c snake.h
	gcc -c -g snake.c

set_ticker.o : set_ticker.c
	gcc -c -g set_ticker.c

linklist.o : linklist.c linklist.h
	gcc -c -g linklist.c

res.o : res.c res.h
	gcc -c -g res.c

xc_queue.o : xc_queue.c xc_queue.h
	gcc -c -g xc_queue.c

clean :
	rm snake snake.o set_ticker.o linklist.o res.o xc_queue.o