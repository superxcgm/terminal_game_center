#include "xc_graph.h"
#define XC_LINKLIST

/* cur have a pointer that pointes to previous node */
struct node_front{
	struct node_front *prev;
	struct xc_point pos;
};
struct node_front *add_to_head(struct node_front *head,
	const struct xc_point *pos);