#include "linklist.h"
#include <cstdio>
#include <cstdlib>

struct node_front *add_to_head(struct node_front *head,
	const Point *pos)
{
	struct node_front *new_node = (struct node_front *)malloc(
		sizeof(struct node_front));

	new_node->prev = nullptr;
	new_node->pos = *pos;
	head->prev = new_node;
	return new_node;
}
void free_node_front(struct node_front *tail)
{
	struct node_front *prev;
	while(tail){
		prev = tail->prev;
		free(tail);
		tail = prev;
	}
}