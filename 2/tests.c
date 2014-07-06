#ifndef TEST

#include <stdlib.h>
#include <stdio.h>

#define true 1
#define false 0


/*---------- Functions ----------*/
void run_tests();
void test_move_top_block();
void test_pile_over();
void test_pile_onto();
void test_move_over();
void test_move_onto();


/*---------- Implementations ----------*/

void run_tests() {
	test_move_top_block();
	test_pile_over();
	test_pile_onto();
	test_move_over();
	test_move_onto();
}

void test_move_top_block(){
	world_t* world = world_create(3);
	move_top_block(world, 1, 2);
	/* Test case 1 - move block on top of another */
	assert(world->position_blocks_top[1] == NULL &&
			world->position_blocks_bottom[1] == NULL);
	assert(world->position_blocks_top[2]->value == 1 && 
			world->position_blocks_bottom[2]->value == 2);
	assert(equals(block_get(world, 2)->next, block_get(world, 1)));
	assert(equals(block_get(world, 1)->previous, block_get(world, 2)));
	assert(block_get_stack(block_get(world, 1)) == 2);

	/* Test case 2 - try to move slot without block*/
	move_top_block(world, 1, 2);

	/* Test case 3 - block to empty space */
	move_top_block(world, 2, 1);
	assert(equals(world->position_blocks_top[1], block_get(world, 1)));
	assert(equals(world->position_blocks_bottom[1], block_get(world, 1)));
	assert(equals(world->position_blocks_top[2], block_get(world, 2)));
	assert(equals(world->position_blocks_bottom[2], block_get(world, 2)));
	assert(block_get_stack(block_get(world, 1)) == 1);
	assert(block_get(world, 2)->next == NULL);
	assert(block_get(world, 2)->previous == NULL);
	assert(block_get(world, 1)->next == NULL);
	assert(block_get(world, 1)->previous == NULL);

	/* Tear down */
	world_delete(&world);
}

void test_pile_over(){
	/* Set up*/
	world_t* world = world_create(7);
	pile_over(world, 1, 2);
	pile_over(world, 3, 4);
	pile_over(world, 5, 6);

	/* Test case 1 - move all left stack to right stack*/
	pile_over(world, 2, 4);
	int i = 0;
	for(i = 1; i <= 4; i++) {
		assert(block_get_stack(block_get(world, i)) == 4);
	}
	assert(world->position_blocks_bottom[2] == NULL &&
			world->position_blocks_top[2] == NULL);


	/* Test case 2 - move part of left stack to right stack. Choose middle elem in right stack*/
	/* Elements should be on top of 6*/
	pile_over(world, 3, 5);
	int stack[] = {1, 2, 3, 5, 6};
	for(i = 0; i < 5; i++) {
		assert(block_get_stack(block_get(world, stack[i])) == 6);
	}
	assert(world->position_blocks_bottom[4]->value == 4 &&
			world->position_blocks_top[4]->value == 4);
	assert(world->position_blocks_bottom[6]->value == 6 &&
			world->position_blocks_top[6]->value == 1);

	/* Test case 3 - move block that is already on top of another*/
	pile_over(world, 3, 5);
	for(i = 0; i < 5; i++) {
		assert(block_get_stack(block_get(world, stack[i])) == 6);
	}
	assert(world->position_blocks_bottom[4]->value == 4 &&
			world->position_blocks_top[4]->value == 4);
	assert(world->position_blocks_bottom[6]->value == 6 &&
			world->position_blocks_top[6]->value == 1);

	/* Test case 4 - move stack to middle of stack*/
	pile_over(world, 0, 4);
	pile_over(world, 4, 2);
	for (i = 0; i < 7; i++) {
		assert(block_get_stack(block_get(world, i)) == 6);
	}

	/* Tear down */
	world_delete(&world);

}

void test_pile_onto() {
	/* Set up */
	world_t* world = world_create(7);
	pile_onto(world, 1, 2);
	pile_onto(world, 3, 4);
	pile_onto(world, 2, 4);

	/* Test case 1 - Middle of stack A to middle of stack B */
	assert(block_get_stack(block_get(world, 3)) == 3);
	assert(block_get_stack(block_get(world, 1)) == 4 &&
		block_get_stack(block_get(world, 2)) == 4);
	assert(world->position_blocks_top[4]->value == 1 &&
			world->position_blocks_bottom[4]->value == 4);

	/* Test case 2 - stack A already on top of block B*/
	pile_onto(world, 1, 4);
	assert(block_get_stack(block_get(world, 2)) == 4);

	/* Tear down */
	world_delete(&world);
}

void test_move_over() {
	/* Set up */
	world_t* world = world_create(5);

	/* Test case 1 - A is top of stack */
	move_over(world, 1, 2);
	assert(world->position_blocks_top[1] == NULL && 
			world->position_blocks_bottom[1] == NULL);
	assert(world->position_blocks_top[2]->value == 1 &&
			world->position_blocks_bottom[2]->value == 2);

	/* Test case 2 - A in middle of stack, B on top */
	move_over(world, 3, 4);
	move_over(world, 2, 4);
	assert(block_get_stack(block_get(world, 1)) == 1);
	assert(block_get_stack(block_get(world, 2)) == 4 &&
			block_get_stack(block_get(world, 3)) == 4  &&
			block_get_stack(block_get(world, 4)) == 4 );


	assert(world->position_blocks_top[4]->value == 2 &&
			world->position_blocks_bottom[4]->value == 4);

	/* Test case 3 - A and B on same stack */
	move_over(world, 3, 4);
	assert(block_get_stack(block_get(world, 2)) == 4 &&
			block_get_stack(block_get(world, 3)) == 4  &&
			block_get_stack(block_get(world, 4)) == 4 );
	assert(world->position_blocks_top[4]->value == 2 &&
			world->position_blocks_bottom[4]->value == 4);

	/* Tear down */
	world_delete(&world);
}

void test_move_onto() {
	/* Set up */
	world_t* world = world_create(5);

	/* Test case 1 - A is top of stack */
	move_onto(world, 1, 2);
	assert(world->position_blocks_top[1] == NULL && 
			world->position_blocks_bottom[1] == NULL);
	assert(world->position_blocks_top[2]->value == 1 &&
			world->position_blocks_bottom[2]->value == 2);

	/* Test case 2 - A and B in middle of their stacks */
	move_onto(world, 3, 4);
	move_onto(world, 2, 4);
	assert(block_get_stack(block_get(world, 1)) == 1 &&
			block_get_stack(block_get(world, 3)) == 3);
	assert(block_get_stack(block_get(world, 2)) == 4 &&
			block_get_stack(block_get(world, 4)) == 4);

	assert(world->position_blocks_top[4]->value == 2 &&
			world->position_blocks_bottom[4]->value == 4);

	/* Test case 3 - A and B on same stack */
	move_onto(world, 3, 2);
	move_onto(world, 3, 4);
	assert(block_get_stack(block_get(world, 2)) == 4);

	/* Tear down */
	world_delete(&world);
}

#endif