#include "graph.h"
#define XC_LINKLIST

/* cur have a pointer that pointes to previous node */
struct node_front{
	struct node_front *prev;
	Point pos;
};
struct node_front *add_to_head(struct node_front *head,
	const Point *pos);
void free_node_front(struct node_front *tail);