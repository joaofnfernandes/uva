#ifndef MAIN

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "block.c"
#include "world.c"
#include "tests.c"

#define true 1
#define false 0

/*---------- Declarations  ----------*/
world_t* process_input();
void output_response(world_t* world);



int main(int argc, char** argv) {
	//run_tests();
	output_response(process_input());
	return 0;
}


/*------------ Implementation -----------*/
world_t* process_input() {
	int size = 0;
	if(scanf("%d", &size) < 0) {
		fprintf(stderr, "%s\n", "Define how many cubes there are, at the beginning of the file");
	}
	world_t* world = world_create(size);
	char str1[10]; char str2[10];
	int block1 = 0, block2 = 0;
	
	while(true) {
		scanf("%s", str1);
		if (strcmp(str1, "quit") == 0) {
			break;
		}
		scanf("%d %s %d", &block1, str2, &block2);
		/* Pile */
		if (strcmp(str1, "pile") == 0) {
			/* Onto */
			if (strcmp(str2, "onto") == 0) {
				pile_onto(world, block1, block2);
			} 
			/* Over */
			else {
				pile_over(world, block1, block2);
			}
		}
		/* Move */
		else {
			/* Onto */
			if (strcmp(str2, "onto") == 0) {
				move_onto(world, block1, block2);
			}
			/* Over */
			else {
				move_over(world, block1, block2);
			}
		}
	}
	return world;
}

void output_response(world_t* world){
	for (int i = 0; i < world->size; i++) {
		printf("%d:", i);
		world_print_stack(world, i);
		printf("\n");
	}
}

#endif




