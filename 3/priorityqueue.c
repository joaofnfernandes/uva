#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "state.c"

#define true 1
#define false 0
/*
	A priority queue that returns the states that minimize the number of moves
 */

//todo: hash should be a double to avoid overflow?

/*---------- Data types ----------*/
struct node_s {
	state_t* state;
	struct node_s* next;
};
typedef struct node_s node_t;

struct priority_queue_s {
	node_t* to_visit;
	node_t* visited;
};
typedef struct priority_queue_s priority_queue_t;

/*---------- Functions ----------*/
node_t* node_make(state_t* state);
priority_queue_t* queue_create();
void push(priority_queue_t*, state_t*);
state_t* pop(priority_queue_t*);

/*---------- Tests ----------*/
void test_priority_queue();

/*---------- Implementations ----------*/
node_t* node_make(state_t* state) {
	node_t* node = (node_t*) malloc(sizeof(node_t));
	node->state = state;
	node->next = NULL;
	return node;
}

priority_queue_t* queue_create() {
	priority_queue_t* queue = (priority_queue_t*)malloc(sizeof(priority_queue_t));
	queue->to_visit = NULL;
	queue->visited = NULL;
	return queue;
}

int queue_contains(priority_queue_t* queue, state_t* state) {
	//search in visited
	node_t* current = queue->visited;
	while (current != NULL) {
		if (state_equals(current->state, state)) {
			return true;
		}
		current = current->next;
	}
	// searh in to_visit
	current = queue->to_visit;
	while (current != NULL) {
		if(state_equals(current->state, state)){
			return true;
		}
		current = current->next;
	}

	return false;
}

void push(priority_queue_t* queue, state_t* state) {
	//see if state already in visited list
	if(queue_contains(queue, state)) {
		return;
	}
	//insert
	node_t** current = &(queue->to_visit);
	while(*current != NULL && (*current)->state->cost <= state->cost) {
		current = &(*current)->next;
	}
	node_t* node = node_make(state);
	node->next = *current;
	*current = node;
}

state_t* pop(priority_queue_t* queue) {
	node_t* node = queue->to_visit;
	if (node != NULL) {
		queue->to_visit = node->next;
		node->next = queue->visited;
		queue->visited = node;
	}
	return node != NULL ? node->state : NULL;
}

void test_priority_queue() {
	box_t* box = box_create(0,0,0);
	state_t* state0 = state_create(box, box, box); state0->cost = 0;
	state_t* state1 = state_copy(state0); state1->cost = 1;
	state_t* state2 = state_copy(state0); state2->cost = 2;
	state_t* state3 = state_copy(state0); state3->cost = 3;

	priority_queue_t* queue = queue_create();
	push(queue, state2); push(queue, state0); push(queue, state1); push(queue, state3);
	assert(pop(queue)->cost == 2 && pop(queue) == NULL);

}

#endif
