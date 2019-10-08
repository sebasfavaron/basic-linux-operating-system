#ifndef BUDDY_H
#define BUDDY_H

#define TEST

#include <stdio.h>
#include <stdint.h>

#define BLOCK_SIZE 4096
#define NUMBER_OF_BLOCKS 8
#define TOTAL_SIZE (BLOCK_SIZE * NUMBER_OF_BLOCKS)
#define HEAP_SIZE ((NUMBER_OF_BLOCKS * 2) - 1)

#define BLOCK_EMPTY 0
#define BLOCK_SPLITTED 1
#define BLOCK_FULL 2

#define LEFT_BLOCK(i) (2 * (i))
#define RIGHT_BLOCK(i) ((2 * (i)) + 1)
#define PARENT_BLOCK(i) ((i) / 2)
#define SIBLING_BLOCK(i) ((i) % 2 ? ((i) - 1) : ((i) + 1))
#define NEXT_BLOCK(i) ((i) + 1)

typedef struct {
	void * address;
	char used;
	uint64_t size; 
} typeBlockInfo;

typedef struct {
	typeBlockInfo blockInfo[HEAP_SIZE];
	int maxLevel;
} typeHeap;

void initializeBuddyManager();
void * allocBlock(uint64_t size);
int deallocBlock(void * address);
int getRequiredLevel(uint64_t size);
void * findEmptyBlock(int index, int currentLevel, int requiredLevel);
int findUsedBlock(int index);
void mergeEmptySiblings(int index);
uint64_t findUsedBlockSize(void * address);
void printHeap();

#endif