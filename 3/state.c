#ifndef STATE
#define STATE

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define true 1
#define false 0

#define NR_BOXES 3

#define BOX1 0
#define BOX2 1
#define BOX3 2
#define BROWN 0
#define GREEN 1
#define CLEAR 2
// TODO: if nr of bottles is > MAX_INT overflows
// state print


/*---------- Data types ----------*/
/* Represents a box with bottles */
struct box_s {
	int brown;
	int green;
	int clear;
};
typedef struct box_s box_t;
/* Represents the state of three boxes and the bottles they have 
	It is an matrix of boxes/bottles, and the cost of moving bottles
	between boxes to reach this state */
struct state_s {
	int** board;
	int hash;
	int cost;
};
typedef struct state_s state_t;

/*---------- Functions ----------*/
/* Creates a box  with brown, green, and clear bottles */
box_t* box_create(int brown, int green, int clear);
/* Creates a state from three initial boxes with bottles in them */
state_t* state_create(box_t* b1, box_t* b2, box_t* b3);
state_t* state_copy(state_t* state);
int state_equals(state_t* s1, state_t* s2);
int state_compute_hash(state_t* state);
state_t** state_generate_children(state_t*);
int state_is_solution(state_t* state);
/* Chooses state that minimizes changes */
state_t* state_choose_best(state_t** states);
int state_move_bottles(state_t* state, int color, int from, int to);
void state_print(state_t* state);

/*---------- Tests ----------*/
void run_tests();
void test_compute_hash();
void test_state_is_solution();
void test_state_generate_children();
void test_state_print();

/*---------- Implementations ----------*/


box_t* box_create(int brown, int green, int clear) {
	box_t* box = (box_t*) malloc(sizeof(box_t));
	box->brown = brown;
	box->green = green;
	box->clear = clear;
	return box;
}

state_t* state_create(box_t* b1, box_t* b2, box_t* b3) {
	state_t* state = (state_t*) malloc(sizeof(state_t));
	int** board = (int**) malloc(sizeof(int*));
	*board = (int*) malloc(sizeof(int*) * NR_BOXES);
	for (int i = 0; i < NR_BOXES; i++) {
		board[i] = (int*) malloc(sizeof(int) * NR_BOXES);
	}

	board[0][0] = b1->brown;
	board[0][1] = b1->green;
	board[0][2] = b1->clear;
	board[1][0] = b2->brown;
	board[1][1] = b2->green;
	board[1][2] = b2->clear;
	board[2][0] = b3->brown;
	board[2][1] = b3->green;
	board[2][2] = b3->clear;



	state->board = board;
	state->hash = state_compute_hash(state);
	state->cost = 0;
	return state;
}

//TODO: internal changes to state will not recompute hash
state_t* state_copy(state_t* state) {
	state_t* new_state = (state_t*) malloc(sizeof(state_t));
	int** board = (int**) malloc(sizeof(int*) * NR_BOXES);
	new_state->board = board;
	new_state->hash = state->hash;
	new_state->cost = state->cost;
	for (int i = 0; i < NR_BOXES; i++) {
		board[i] = (int*) malloc(sizeof(int) * NR_BOXES);
	}
	for (int i = 0; i < NR_BOXES; i++){
		for (int j = 0; j < NR_BOXES; j++) {
			new_state->board[i][j] = state->board[i][j];
		}
	}
	return new_state;
}

int state_equals(state_t* s1, state_t* s2) {
	if ((s1 == NULL && s2 != NULL) || (s1 != NULL && s2 == NULL)) {
		return false;
	}
	if (s1->hash != s2->hash) {
		return false;
	}
	for (int i = 0; i < NR_BOXES; i++) {
		for (int j = 0; j < NR_BOXES; j++)
		{
			if(s1->board[i][j] != s2->board[i][j]){
				return false;
			}
		}
	}
	return true;
}

int state_compute_hash(state_t* state) {
	int hash = 0, line = 0;
	for (int i = 0; i < NR_BOXES; i++) {
		line = 0;
		for (int j = 0; j < NR_BOXES; j++) {
			line += (i * NR_BOXES) + j + state->board[i][j] + 1;
		}
		hash = hash ^ line;
	}
	return hash;
}

void test_compute_hash(){
	// Setup
	box_t* box1 = box_create(0,1,2);
	box_t* box2 = box_create(0,1,1);
	state_t* state1 = state_create(box1, box1, box1);
	state_t* state2 = state_create(box1, box1, box2);


	// Test case 1 - states are the same
	assert(state_compute_hash(state1) == state_compute_hash(state1));

	// Test case  -  states are different
	assert(state_compute_hash(state1) != state_compute_hash(state2));

	// Teardown
}
//todo, why 19 ? - someone is testing that position for null?
state_t** state_generate_children(state_t* state) {
	/* Allocate 6 * 3 states */
	int TOTAL_CHILDREN = 19;
	state_t** states = (state_t**)malloc(sizeof(state_t*) * TOTAL_CHILDREN);
	for(int i = 0; i < TOTAL_CHILDREN; i++) {
		states[i] = NULL;
	}

	int states_added = 0;
	int swaps[12] = {BOX1, BOX2,
					BOX1, BOX3,
					BOX2, BOX1,
					BOX2, BOX3,
					BOX3, BOX1,
					BOX3, BOX2};
	state_t* new_state = state_copy(state);
	for (int i = 0; i < NR_BOXES; i++){
		for(int j = 0; j < 12; j += 2) {
			if(state_move_bottles(new_state, i, swaps[j], swaps[j + 1])){
				states[states_added++] = new_state;
				new_state = state_copy(state);
			}
		}
	}
	return states[0] != NULL ? states : NULL;
}
//TODO fix tests
void test_state_generate_children() {
	// Setup
	box_t* box1 = box_create(0,0,0);
	box_t* box2= box_create(0,0,1);
	box_t* box3= box_create(0,1,1);
	state_t* state1 = state_create(box1, box1, box1);
	state_t* state2 = state_create(box1, box1, box2);
	state_t* state3 = state_create(box1, box1, box3);
	state_t** children = NULL;

	// Test case 1 - solved state should generate no children
	children = state_generate_children(state1);
	assert(children == NULL);

	// Test case 2 - state should generate no children
	// todo: this is returning wrong states
	children = state_generate_children(state2);
	assert(children == NULL);

	//Test case 3 - state should generate 2 states
	//000  000
	//000  000
	//002  020
	children = state_generate_children(state3);
	assert(children != NULL);
	box_t* s1= box_create(0,0,2);
	box_t* s2= box_create(0,2,0);
	assert(state_equals(children[0],state_create(box1,box1,s1)));
	assert(state_equals(children[1],state_create(box1,box1,s2)));
	assert(children[2] == NULL);

	// Teardown
}

int state_move_bottles(state_t* state, int color, int from, int to) {
	if(state->board[color][from] == 0) {
		return false;
	}
	state->cost += state->board[color][from];
	state->board[color][to] += state->board[color][from];
	state->board[color][from] = 0;
	return true;
}

int state_is_solution(state_t* state) {
	int boxes[NR_BOXES];
	for(int i = 0; i < NR_BOXES; i++) {
		boxes[i] = 0;
	}
	for (int i = 0; i < NR_BOXES; i++) {
		for (int j = 0; j < NR_BOXES; j++) {
			if (state->board[i][j] != 0) {
				if(boxes[j] == 0) {
					boxes[j] = state->board[i][j];
				} else {
					return false;
				}
			}
		}
	}
	return true;
}

void test_state_is_solution() {
	// Setup
	box_t* box1 = box_create(0,0,1);
	box_t* box2 = box_create(0,1,1);
	state_t* solved_state = state_create(box1, box1, box1);
	state_t* not_solved_state = state_create(box2, box1, box1);

	// Test case 1 - state is solution
	assert(state_is_solution(solved_state));

	// Test case 2 - state is not solution
	assert(state_is_solution(not_solved_state) == false);
	// Teardown
}

state_t* state_choose_best(state_t** states) {
	state_t* current = states[0];
	state_t* min = states[0];
	int i = 1;
	while (current != NULL) {
		if(states[i]->cost < min->cost) {
			min = current;
		}
		current = states[++i];
	}
	return min;
}

void state_print(state_t* state) {
	printf("%d\n",state->cost );
	for (int i = 0; i < NR_BOXES; i++) {
		for (int j = 0; j < NR_BOXES; j++) {
			printf(" %d", state->board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void test_state_print() {
	box_t* box1 = box_create(0,0,1);
	state_t* solved_state = state_create(box1, box1, box1);
	state_print(solved_state);
}

void run_tests() {
	test_compute_hash();
 	test_state_is_solution();
 	test_state_generate_children();
}

#endif