#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct entry {
	int key;
	int value;
	struct entry* next;
};

struct hashTable{
	int size;
	struct entry** table;
};


int 				hash(struct hashTable*, int );
struct entry* 		ht_newpair(int, int);
struct hashTable* 	ht_create(int size);
void 				ht_add(struct hashTable*, int, int);
int 				ht_get(struct hashTable*, int);
void				ht_delete(struct hashTable**);


void test_ht_create();
void test_ht_add();
void run_tests();


struct hashTable* ht_create(int size){
	if(size < 1) {
		return NULL;
	}
	struct hashTable* table = NULL;

	//Allocate table
	if((table = malloc(sizeof(struct hashTable))) == NULL){
		return NULL;
	}

	table->size = size;
	//Allocate head nodes
	if((table->table = malloc(sizeof(struct entry*) * size)) == NULL){
		return NULL;
	}
	for(int i = 0; i < size; i++){
		table->table[i] = NULL;
	}
	return table;
}

int hash(struct hashTable* table, int value){
	return value % table->size;
}

struct entry* ht_newpair(int key, int value){
	struct entry* pair = NULL;
	if((pair = malloc(sizeof(struct entry))) == NULL){
		return NULL;
	}
	pair->key = key;
	pair->value = value;
	pair->next = NULL;
	return pair;
}

void ht_add(struct hashTable* table, int key, int value){
	if(table == NULL){
		return;
	}
	int bin = hash(table, key);
	struct entry* last = NULL;
	struct entry* current = table->table[bin];
	/*iterate until finding a pair with the same key*/
	while(current != NULL && current->key != key){
		last = current;
		current = current->next;
	}
	/*we haven't found a pair with same key, add pair to the beginning*/
	if(current == NULL){
		struct entry* pair = ht_newpair(key, value);
		pair->next = table->table[bin];
		table->table[bin] = pair;
	} else {
		/*overwrite value*/
		current->value = value;
	}
}

int ht_get(struct hashTable* table, int key){
	if(table == NULL){
		return -1;
	}
	int bin = hash(table, key);
	struct entry* current = table->table[bin];
	while(current != NULL && current->key != key){
		current = current->next;
	}
	if(current == NULL){
		return -1;
	} else {
		return current->value;
	}
}

void ht_delete(struct hashTable** table) {
	for(int bin = 0; bin < (*table)->size; bin++){
		struct entry* pair = NULL;
		while((*table)->table[bin] != NULL){
			pair = (*table)->table[bin];
			(*table)->table[bin] = (*table)->table[bin]->next;
			free(pair);
		}
	}
	free((*table)->table);
	free(*table);
	*table = NULL;
}

/*int main (int argc, char** argv){
	run_tests();
	return 0;
}*/

void run_tests(){
	test_ht_create();
	test_ht_add();
}

void test_ht_create(){
	struct hashTable* table = NULL;
	table = ht_create(5);
	assert(table->size = 5);
	for (int i = 0; i < table->size; ++i){
		assert(table->table[i] == NULL);
	}
}

void test_ht_add(){
	struct hashTable* table = ht_create(-1);
	assert(table == NULL);
	table = ht_create(5);
	assert(table->size = 5);
	for(int i = 0; i < 10; i++){
		ht_add(table, i, i);
		assert(ht_get(table, i) == i);
	}
	/*insert duplicate*/
	ht_add(table, 1, 100);
	assert(ht_get(table, 1) == 100);
	ht_delete(&table);
	assert(table == NULL);
}


