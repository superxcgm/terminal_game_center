#include <stdio.h>
#include "xc_queue.h"

/* return: 1=>success, 0=>failed */

int xc_queue_init(struct xc_queue *queue)
{
	if(queue == NULL) return 0;
	queue->head = queue->tail = 0;
	return 1;
}

int xc_queue_insert(struct xc_queue *queue, int x)
{
	if(queue == NULL) return 0;
	/* queue not full */
	if(queue->head + 1 != queue->tail && !(queue->head + 1 == QUEUE_MAX && queue->tail == 0)){
		queue->data[queue->head++] = x;
		if(queue->head == QUEUE_MAX) queue->head = 0;
		return 1;
	}
	return 0;
}

int xc_queue_get(struct xc_queue *queue, int *value)
{
	if(queue == NULL) return 0;
	/* no data in the queue */
	if(queue->head == queue->tail) return 0;
	*value = queue->data[queue->tail++];
	if(queue->tail == QUEUE_MAX) queue->tail = 0;
	return 1;
}