#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "hash.c"

#define true 1
#define false 0


int cycleLength(struct hashTable*, int);
int maxCycleLength(struct hashTable*, int, int);
int memoizedMaxCycleLength(int, int);

void testMaxCycleLength();

int main(int argc, char** argv){
	int i, j = 0;
	while(scanf("%d %d", &i, &j)  >= 0){
		printf("%d %d %d \n", i, j, memoizedMaxCycleLength(i, j));
	}
	return 0;
}

int cycleLength(struct hashTable* dictionary, int n){
	int value = ht_get(dictionary, n);
	if(value != -1){
		return value;
	}
	if(n == 1){
		return 1;
	} else if(n % 2 == 0){
		return 1 + cycleLength(dictionary, n/2);
	} else {
		return 1 + cycleLength(dictionary, 3*n + 1);
	}
}

int maxCycleLength(struct hashTable* dictionary, int i, int j){
	int max = 0, length = 0, k = 0;
	if(i > j){
		int aux = i;
		i = j;
		j = aux;
	}
	for(k = i; k <= j; k++) {
		if(k % 2 == 1){
			length = cycleLength(dictionary, k);
		} else {
			if(k == j){
				length = cycleLength(dictionary, k);
			}
		}
		max = length > max ? length : max;
	}
	return max;
}

int memoizedMaxCycleLength(int i, int j){
	struct hashTable* dictionary = ht_create(50);
	int value = maxCycleLength(dictionary, i, j);
	ht_delete(&dictionary);
	return value;
}

void testMaxCycleLength(){
	assert(memoizedMaxCycleLength(1, 10) == 20);
	assert(memoizedMaxCycleLength(100, 200) == 125);
	assert(memoizedMaxCycleLength(201, 210) == 89);
	assert(memoizedMaxCycleLength(900, 1000) == 174);
}

