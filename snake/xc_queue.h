#define QUEUE_MAX 50

struct xc_queue{
	int data[QUEUE_MAX];
	int head;
	int tail;
};

int xc_queue_init(struct xc_queue *queue);
int xc_queue_insert(struct xc_queue *queue, int x);
int xc_queue_get(struct xc_queue *queue, int *value);