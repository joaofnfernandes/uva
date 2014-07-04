#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define true 1
#define false 0

/*---------- Data types ----------*/
struct node_s {
	int value;
	int current_stack;
	struct node_s* next;
	struct node_s* previous;
};
typedef struct node_s node_t;

/*Encapsulates all the world state*/
struct world_s {
	/*Direct pointer to a block*/
	node_t** blocks;
	/*Stack of blocks. From top to bottom*/
	node_t** position_blocks_top;
	/*Stack of blocks. From bottom to top*/
	node_t** position_blocks_bottom;
};
typedef struct world_s world_t;

/*---------- Functions ----------*/
/* Manipulate Blocks */
/* Move one stack on top of another */
void pile_over(world_t* world, int a, int b);
/* Move a stack directly on top of a block */
void pile_onto(world_t* world, int a, int b);
/* Add block A on top of stack containing B */
void move_over(world_t* world, int a, int b);
/* Add block A on top of block B */
void move_onto(world_t* world, int a, int b);
void run();

/* Auxiliary */
node_t* node_create(int value);
world_t* world_create(int size);
/* Gets a pointer to the block */
node_t* block_get(world_t* world, int b);
/* Gets the stack in which the block is currently at */
int block_get_stack(world_t* world, int b);
void block_set_stack(node_t* n, int i);
/* Move the top block from one stack to another */
void move_top_block(world_t* world, int a, int b);
/* Compares to blocks */
int equals(node_t* a, node_t* b);
/* Checks if one block on on top of another */
int block_is_on_top(world_t* world, node_t* a, node_t* b);

//TODO: delete blocks and delete world

/* Tests*/
void run_tests();
void test_move_top_block();
void test_pile_over();
void test_pile_onto();
void test_move_over();
void test_move_onto();

/*---------- Main ----------*/

int main(int argc, char** argv) {
	run();
	return 0;
}

/*---------- Implementations ----------*/

void run() {
	int size = 0;
	if(scanf("%d", &size) < 0) {
		fprintf(stderr, "%s\n", "Define how many cubes there are, at the beginning of the file");
	}
	world_t* world = world_create(size);
	char str1[10]; char str2[10];
	int cube1 = 0, cube2 = 0;
	
	while(true) {
		scanf("%s", str1);
		if (strcmp(str1, "quit") == 0) {
			break;
		}
		scanf("%d %s %d", &cube1, str2, &cube2);
		/* Pile */
		if (strcmp(str1, "pile") == 0) {
			/* Onto */
			if (strcmp(str2, "onto")) {
				pile_onto(world, cube1, cube2);
			} 
			/* Over */
			else {
				pile_over(world, cube1, cube2);
			}
		}
		/* Move */
		else {
			/* Onto */
			if (strcmp(str2, "onto")) {
				move_onto(world, cube1, cube2);
			}
			/* Over */
			else {
				move_over(world, cube1, cube2);
			}
		}
	}
}

void run_tests() {
	test_move_top_block();
	test_pile_over();
	test_pile_onto();
	test_move_over();
	test_move_onto();
}

node_t* node_create(int value) {
	node_t* node = (node_t*) malloc(sizeof(node_t));
	node->value = value;
	node->current_stack = value;
	node->next = NULL;
	node->previous = NULL;
	return node;
}

world_t* world_create(int size) {
	if(size <= 0) {
		return NULL;
	}
	/*allocations. we'll use 1-based indexing. So position[0] has no meaning*/
	world_t* world = (world_t*) malloc(sizeof(world_t));
	world->blocks = (node_t**) malloc(sizeof(node_t) * (size + 1)); 
	world->position_blocks_top = (node_t**) malloc(sizeof(node_t) * (size + 1)); 
	world->position_blocks_bottom = (node_t**) malloc(sizeof(node_t) * (size + 1)); 
	/*populate*/
	int i = 0; 
	node_t* new_node = NULL;
	for(i = 1; i <= size; i++) {
		new_node = node_create(i);
		world->blocks[i] = new_node;
		world->position_blocks_top[i] = new_node;
		world->position_blocks_bottom[i] = new_node;
	}
	return world;
}

node_t* block_get(world_t* world, int b) {
	return world->blocks[b];
}

int block_get_stack(world_t* world, int b) {
	return (block_get(world, b))->current_stack;
}

void block_set_stack(node_t* n, int i) {
	n->current_stack = i;
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

void test_move_top_block(){
	world_t* world = world_create(2);
	move_top_block(world, 1, 2);
	/* Test case 1 - move block on top of another */
	assert(world->position_blocks_top[1] == NULL &&
			world->position_blocks_bottom[1] == NULL);
	assert(world->position_blocks_top[2]->value == 1 && 
			world->position_blocks_bottom[2]->value == 2);
	assert(equals(block_get(world, 2)->next, block_get(world, 1)));
	assert(equals(block_get(world, 1)->previous, block_get(world, 2)));
	assert(block_get_stack(world, 1) == 2);

	/* Test case 2 - try to move slot without block*/
	move_top_block(world, 1, 2);

	/* Test case 3 - block to empty space */
	move_top_block(world, 2, 1);
	assert(equals(world->position_blocks_top[1], block_get(world, 1)));
	assert(equals(world->position_blocks_bottom[1], block_get(world, 1)));
	assert(equals(world->position_blocks_top[2], block_get(world, 2)));
	assert(equals(world->position_blocks_bottom[2], block_get(world, 2)));
	assert(block_get_stack(world, 1) == 1);
	assert(block_get(world, 2)->next == NULL);
	assert(block_get(world, 2)->previous == NULL);
	assert(block_get(world, 1)->next == NULL);
	assert(block_get(world, 1)->previous == NULL);
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

void test_pile_over(){
	/* Set up*/
	world_t* world = world_create(6);
	pile_over(world, 1, 2);
	pile_over(world, 3, 4);
	pile_over(world, 5, 6);

	/* Test case 1 - move all left stack to right stack*/
	pile_over(world, 2, 4);
	for(int i = 1; i <= 4; i++) {
		assert(block_get_stack(world, i) == 4);
	}
	assert(world->position_blocks_bottom[2] == NULL &&
			world->position_blocks_top[2] == NULL);


	/* Test case 2 - move part of left stack to right stack. Choose middle elem in right stack*/
	/* Elements should be on top of 6*/
	pile_over(world, 3, 5);
	int stack[] = {1, 2, 3, 5, 6};
	for(int i = 0; i < 5; i++) {
		assert(block_get_stack(world, stack[i]) == 6);
	}
	assert(world->position_blocks_bottom[4]->value == 4 &&
			world->position_blocks_top[4]->value == 4);
	assert(world->position_blocks_bottom[6]->value == 6 &&
			world->position_blocks_top[6]->value == 1);

	/* Test case 3 - move block that is already on top of another*/
	pile_over(world, 3, 5);
	for(int i = 0; i < 5; i++) {
		assert(block_get_stack(world, stack[i]) == 6);
	}
	assert(world->position_blocks_bottom[4]->value == 4 &&
			world->position_blocks_top[4]->value == 4);
	assert(world->position_blocks_bottom[6]->value == 6 &&
			world->position_blocks_top[6]->value == 1);

	//TODO teardown

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

//TODO: A is beneath B
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

// TODO test case where A is beneath B
void test_pile_onto() {
	/* Set up */
	world_t* world = world_create(6);
	pile_onto(world, 1, 2);
	pile_onto(world, 3, 4);
	pile_onto(world, 2, 4);

	/* Test case 1 - Middle of stack A to middle of stack B */
	assert(block_get_stack(world, 3) == 3);
	assert(block_get_stack(world, 1) == 4 &&
			block_get_stack(world, 2) == 4);
	assert(world->position_blocks_top[4]->value == 1 &&
			world->position_blocks_bottom[4]->value == 4);

	/* Test case 2 - stack A already on top of block B*/
	pile_onto(world, 1, 4);
	assert(block_get_stack(world, 2) == 4);	

}

// todo A beneath B
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

void test_move_over() {
	/* Set up */
	world_t* world = world_create(4);

	/* Test case 1 - A is top of stack */
	move_over(world, 1, 2);
	assert(world->position_blocks_top[1] == NULL && 
			world->position_blocks_bottom[1] == NULL);
	assert(world->position_blocks_top[2]->value == 1 &&
			world->position_blocks_bottom[2]->value == 2);

	/* Test case 2 - A in middle of stack, B on top */
	move_over(world, 3, 4);
	move_over(world, 2, 4);
	assert(block_get_stack(world, 1) == 1);
	assert(block_get_stack(world, 2) == 4 &&
			block_get_stack(world, 3) == 4 &&
			block_get_stack(world, 4) == 4);
	assert(world->position_blocks_top[4]->value == 2 &&
			world->position_blocks_bottom[4]->value == 4);

	/* Test case 3 - A and B on same stack */
	move_over(world, 3, 4);
	assert(block_get_stack(world, 2) == 4 &&
			block_get_stack(world, 3) == 4 &&
			block_get_stack(world, 4) == 4);
	assert(world->position_blocks_top[4]->value == 2 &&
			world->position_blocks_bottom[4]->value == 4);

	/* Tear down */

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

void test_move_onto() {
	/* Set up */
	world_t* world = world_create(4);

	/* Test case 1 - A is top of stack */
	move_onto(world, 1, 2);
	assert(world->position_blocks_top[1] == NULL && 
			world->position_blocks_bottom[1] == NULL);
	assert(world->position_blocks_top[2]->value == 1 &&
			world->position_blocks_bottom[2]->value == 2);

	/* Test case 2 - A and B in middle of their stacks */
	move_onto(world, 3, 4);
	move_onto(world, 2, 4);
	assert(block_get_stack(world, 1) == 1 && 
			block_get_stack(world, 3) == 3);
	assert(block_get_stack(world, 2) == 4 &&
			block_get_stack(world, 4) == 4);
	assert(world->position_blocks_top[4]->value == 2 &&
			world->position_blocks_bottom[4]->value == 4);

	/* Test case 3 - A and B on same stack */
	move_onto(world, 3, 2);
	move_onto(world, 3, 4);
	assert(block_get_stack(world, 2) == 4);

	/* Tear down */
}




