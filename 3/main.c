#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "state.c"
#include "priorityqueue.c"

#define true 1
#define false 0


//add initial state to list of states to explore
//list of explored states is empty
//while there are states to explore
//	current = pop
//	if current is solution, return
//	else, generate states from current state, and add them to list to explore


/*---------- Functions ----------*/
state_t* create_initial_state();
void run();
void print_solution(state_t* state);

/*---------- Tests ----------*/


/*---------- Implementations ----------*/

int main (int argc, char** argv) {
	run();
	//run_tests();
}

void run() {
	priority_queue_t* queue = queue_create();
	push(queue, create_initial_state());
	state_t* current = pop(queue);
	state_t** children = NULL;
	while(current != NULL) {
		//state_print(current);
		if (state_is_solution(current)) {
			print_solution(current);
			return;
		}
		children = state_generate_children(current);
		while (children != NULL && *children != NULL) {
			push(queue, *children);
			children++;
		}
		current = pop(queue);
	}	
}

state_t* create_initial_state() {
	
	int values[9];
	for (int i = 0; i < 9; i++) {
		if(scanf("%d", &values[i]) < 0 ){
			fprintf(stderr, "%s\n", "Please specify a valid initial state");
		}
	}
	box_t* box1 = box_create(values[0], values[1], values[2]);
	box_t* box2 = box_create(values[3], values[4], values[5]);
	box_t* box3 = box_create(values[6], values[7], values[8]);
	state_t* state = state_create(box1, box2, box3);
	return state;
}

void print_solution(state_t* state) {
	char color[NR_BOXES];
	for(int i = 0; i < NR_BOXES; i++) {
		color[i] = 0;
	}
	for(int j = 0; j < NR_BOXES; j++) {
		for(int i = 0; i < NR_BOXES; i++) {
			if(state->board[i][j] != 0) {
				switch(j) {
					case 0:
						color[j] = 'B';
						break;
					case 1:
						color[j] = 'G';
						break;
					case 2:
						color[j] = 'C';
						break;
				}
			}
		}
	}
	for (int i = 0; i < NR_BOXES; i++) {
		printf("%c", color[i]);
	}
	printf(" %d\n", state->cost);
}














