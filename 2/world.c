#ifndef WORLD

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define true 1
#define false 0


/*Encapsulates all the world state*/
struct world_s {
	int size;
	/*Direct pointer to a block*/
	node_t** blocks;
	/*Stack of blocks. From top to bottom*/
	node_t** position_blocks_top;
	/*Stack of blocks. From bottom to top*/
	node_t** position_blocks_bottom;
};
typedef struct world_s world_t;

/*---------- Declarations  ----------*/

world_t* world_create(int size);
void world_delete(world_t** world);

/* Move one stack on top of another */
void pile_over(world_t* world, int a, int b);
/* Move a stack directly on top of a block */
void pile_onto(world_t* world, int a, int b);
/* Add block A on top of stack containing B */
void move_over(world_t* world, int a, int b);
/* Add block A on top of block B */
void move_onto(world_t* world, int a, int b);

/* Auxiliary */
/* Gets a pointer to the block */
node_t* block_get(world_t* world, int b);
/* Move the top block from one stack to another */
void move_top_block(world_t* world, int a, int b);
/* Checks if one block on on top of another */
int block_is_on_top(world_t* world, node_t* a, node_t* b);
/* Prints the cubes in a stack */
void world_print_stack(world_t* world, int i);

/*------------ Implementation -----------*/

world_t* world_create(int size) {
	if(size <= 0) {
		return NULL;
	}
	world_t* world = (world_t*) malloc(sizeof(world_t));
	world->blocks = (node_t**) malloc(sizeof(node_t) * (size)); 
	world->position_blocks_top = (node_t**) malloc(sizeof(node_t) * (size)); 
	world->position_blocks_bottom = (node_t**) malloc(sizeof(node_t) * (size)); 
	/*populate*/
	world->size = size;
	int i = 0; 
	node_t* new_node = NULL;
	for(i = 0; i < size; i++) {
		new_node = node_create(i);
		world->blocks[i] = new_node;
		world->position_blocks_top[i] = new_node;
		world->position_blocks_bottom[i] = new_node;
	}
	return world;
}

void world_delete(world_t** world) {
	free((*world)->position_blocks_top);
	(*world)->position_blocks_top = NULL;
	free((*world)->position_blocks_bottom);
	(*world)->position_blocks_bottom = NULL;
	for (int i = 0; i < (*world)->size; i++) {
		free((*world)->blocks[i]);
		(*world)->blocks[i] = NULL;
	}
	free((*world)->blocks);
	(*world)->blocks = NULL;
}

node_t* block_get(world_t* world, int b) {
	return world->blocks[b];
}

/* Moves the top block of one stack to another. A and B are slot numbers */
void move_top_block(world_t* world, int a, int b) {
	node_t* block_a = world->position_blocks_top[a];
	node_t* block_b = world->position_blocks_top[b];
	/* No block to move */
	if(block_a == NULL || a == b){
		return;
	}
	/* Fix pointers to top of stack*/
	world->position_blocks_top[a] = block_a->previous;
	world->position_blocks_top[b] = block_a;
	
	/* Fix pointers to bottom of stack*/
	if(world->position_blocks_top[a] == NULL){
		world->position_blocks_bottom[a] = NULL;
	}
	if(world->position_blocks_bottom[b] == NULL){
		world->position_blocks_bottom[b] = world->position_blocks_top[b];
	}

	/* Fix make new A top point to null */
	if(block_a->previous != NULL){
		block_a->previous->next = NULL;
	}
	/* Fix A previous */
	block_a->previous = block_b;
	/* Fix old B top */
	if(block_b != NULL){
		block_b->next = block_a;	
	}
	
	/* Fix A's stack */
	block_a->current_stack = b;
}

void pile_over(world_t* world, int a, int b) {
	
	node_t* block_a = block_get(world, a);
	node_t* block_b = block_get(world, b);
	a = block_get(world, a)->current_stack;
	b = block_get(world, b)->current_stack;
	node_t* a_top = world->position_blocks_top[a];
	node_t* b_top = world->position_blocks_top[b];

	/* If blocks already on top of one another, do nothing*/
	if (a == b) {
		return;
	}

	/* Fix pointers to top of stack*/
	world->position_blocks_top[a] = block_a->previous;
	world->position_blocks_top[b] = a_top;
	
	/* Fix pointers to bottom of stack*/
	if(world->position_blocks_top[a] == NULL){
		world->position_blocks_bottom[a] = NULL;
	}
	if(world->position_blocks_bottom[b] == NULL){
		world->position_blocks_bottom[b] = block_a;
	}

	/* Fix make new A top point to null */
	if(block_a->previous != NULL){
		block_a->previous->next = NULL;
	}
	/* Fix A previous */
	block_a->previous = b_top;
	/* Fix old B top */
	if(b_top != NULL){
		b_top->next = block_a;	
	}
	
	/* Fix moved blocks internal info */
	node_t* current_node = block_a;
	while(current_node != NULL){
		block_set_stack(current_node, b);
		current_node = current_node->next;
	}

}

//todo interface should be (world, block#, block#)
int block_is_on_top(world_t* world, node_t* a, node_t* b){
	if (a == NULL || b == NULL) {
		return false;
	}
	node_t* current = b;
	while (current != NULL && !equals(a, current)) {
		current = current->next;
	}
	return current != NULL ? true : false;
}

void pile_onto(world_t* world, int a, int b) {

	node_t* block_a = block_get(world, a);
	node_t* block_b = block_get(world, b);
	a = block_a->current_stack;
	b = block_b->current_stack;
	node_t* a_top = world->position_blocks_top[a];
	node_t* b_top = world->position_blocks_top[b];

	/* If blocks already on top of one another, do nothing */
	if (a == b) {
		return;
	}

	/* Break stack A into two. The top stack will be moved */
	world->position_blocks_top[a] = block_a->previous;
	if(block_a->previous != NULL){
		block_a->previous->next = NULL;
	} else {
		world->position_blocks_bottom[a] = NULL;
	}

	/* Make B the top of its stack */
	node_t* current_node = world->position_blocks_top[b];
	while(!equals(current_node, block_b)){
		current_node = current_node->previous;
		move_top_block(world, b, world->position_blocks_top[b]->value);
	}

	/* Move A stack on top of B */
	world->position_blocks_top[b] = a_top;
	block_a->previous = block_b;
	if(block_b != NULL) {
		block_b->next = block_a;
	} else {
		world->position_blocks_bottom[b] = block_b;
	}

	/* Fix moved blocks internal information */
	node_t* current_block = block_b;
	while (current_block != NULL) {
		current_block->current_stack = b;
		current_block = current_block->next;
	}
}

void move_over(world_t* world, int a, int b) {
	node_t* block_a = block_get(world, a);
	node_t* block_b = block_get(world, b);
	a = block_a->current_stack;
	b = block_b->current_stack;
	node_t* a_top = world->position_blocks_top[a];
	node_t* b_top = world->position_blocks_top[b];

	/* If blocks already on top of one another, do nothing */
	if (a == b) {
		return;
	}

	/* Make A the top of its stack */
	node_t* current_node = a_top;
	int move_to = 0;
	while (!equals(current_node, block_a) && current_node!= NULL) {
		move_to = current_node->value;
		current_node = current_node->previous;
		move_top_block(world, a, move_to);
	}
	/* Pile A onto B */
	pile_over(world, block_a->value, block_b->value);
}


void move_onto(world_t* world, int a, int b) {
	node_t* block_a = block_get(world, a);
	node_t* block_b = block_get(world, b);
	a = block_a->current_stack;
	b = block_b->current_stack;
	node_t* a_top = world->position_blocks_top[a];
	node_t* b_top = world->position_blocks_top[b];

	/* If blocks on same stack, do nothing */
	if (a == b) {
		return;
	}

	/* Make A the top of its stack */
	node_t* current_node = a_top;
	int move_to = 0;
	while (!equals(current_node, block_a) && current_node!= NULL) {
		move_to = current_node->value;
		current_node = current_node->previous;
		move_top_block(world, a, move_to);
	}
	/* Pile A onto B */
	pile_onto(world, block_a->value, block_b->value);
}

void world_print_stack(world_t* world, int i) {
	node_t* current_block = world->position_blocks_bottom[i];
	while (current_block != NULL) {
		printf(" %d", current_block->value);
		current_block = current_block->next;
	}

}


#endif