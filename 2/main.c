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
void run();



int main(int argc, char** argv) {
	//run_tests();
	run();
	return 0;
}


/*------------ Implementation -----------*/
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



#endif




