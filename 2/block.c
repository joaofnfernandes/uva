#ifndef BLOCK

#include <stdlib.h>
#include <stdio.h>

#define true 1
#define false 0



/*---------- Data types ----------*/
/* Represents a block that is below/on top of others */
struct node_s {
	int value;
	int current_stack;
	struct node_s* next;
	struct node_s* previous;
};
typedef struct node_s node_t;


/*---------- Functions ----------*/
node_t* node_create(int value);
void node_delete(node_t** node);
/* Gets the stack in which the block is currently at */
int block_get_stack(node_t* block);
void block_set_stack(node_t* n, int i);
/* Compares to blocks */
int equals(node_t* a, node_t* b);

/*---------- Implementations ----------*/
node_t* node_create(int value) {
	node_t* node = (node_t*) malloc(sizeof(node_t));
	node->value = value;
	node->current_stack = value;
	node->next = NULL;
	node->previous = NULL;
	return node;
}

void node_delete (node_t** node) {
	free(*node);
	*node = NULL;
}

int block_get_stack(node_t* block) {
	return block->current_stack;
}

void block_set_stack(node_t* n, int i) {
	n->current_stack = i;
}

int equals(node_t* a, node_t* b){
	if( a == NULL || b == NULL) {
		if (a == NULL && b == NULL) {
			return true;
		} else {
			return false;
		}
	} else {
		return a->value == b->value ? true : false;
	}
	
}


#endif