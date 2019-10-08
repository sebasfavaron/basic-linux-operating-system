#include <buddyMemoryAlloc.h>

static typeHeap heap;
static void * baseAddress = (void *) 0x600000;

void initializeBuddyManager() {
	int i;
	int level = 1;
	int currentPositionInLevel = 0;
	int maxPositionInLevel = 1;

	for(i = 0; i < HEAP_SIZE; i++) {
		if (currentPositionInLevel == maxPositionInLevel) {
			level++;
			currentPositionInLevel = 0;
			maxPositionInLevel *= 2;
		}

		heap.blockInfo[i].address = baseAddress + (TOTAL_SIZE / maxPositionInLevel) * currentPositionInLevel;
		heap.blockInfo[i].used = BLOCK_EMPTY;
		heap.blockInfo[i].size = TOTAL_SIZE / maxPositionInLevel;
		currentPositionInLevel++;
	}

	heap.maxLevel = level;

	#ifndef TEST
		memset(baseAddress, '\0', TOTAL_SIZE);
	#endif
}

void * allocBlock(uint64_t size) {
	if (size == 0 || size > TOTAL_SIZE)
		return NULL;

	int requiredLevel = getRequiredLevel(size);
	return findEmptyBlock(1, 1, requiredLevel);
}

int deallocBlock(void * address) {
	if (address == NULL || address < baseAddress || address >= baseAddress + TOTAL_SIZE)
		return -1;

	int index = (address - baseAddress) / BLOCK_SIZE + (HEAP_SIZE / 2);
	index = findUsedBlock(index + 1);
	if (index == 0)
		return -1;

	heap.blockInfo[index - 1].used = BLOCK_EMPTY;

	#ifndef TEST
		memset(heap.blockInfo[index - 1].address, '\0', heap.blockInfo[index - 1].size);
	#endif

	mergeEmptySiblings(index);
	return 0;
}

int getRequiredLevel(uint64_t size) {
	int order = 0;
	int blockSize = BLOCK_SIZE;

	while (size > blockSize) {
		order++;
		blockSize *= 2;
	}

	return heap.maxLevel - order;
}

void * findEmptyBlock(int index, int currentLevel, int requiredLevel) {
	if (heap.blockInfo[index - 1].used == BLOCK_FULL) {
		return NULL;
	}

	if (currentLevel == requiredLevel) {
		if (heap.blockInfo[index - 1].used == BLOCK_SPLITTED) {
			return NULL;
		}

		heap.blockInfo[index - 1].used = BLOCK_FULL;
		return heap.blockInfo[index - 1].address;
	}

	void * returnBlock = NULL;

	void * leftBlock = findEmptyBlock(LEFT_BLOCK(index), currentLevel + 1, requiredLevel);
	if (leftBlock != NULL)
		returnBlock = leftBlock;
	else {
		void * rightBlock = findEmptyBlock(RIGHT_BLOCK(index), currentLevel + 1, requiredLevel);
		if (rightBlock != NULL)
			returnBlock = rightBlock;
	}

	if (heap.blockInfo[LEFT_BLOCK(index) - 1].used == BLOCK_FULL && heap.blockInfo[RIGHT_BLOCK(index) - 1].used == BLOCK_FULL)
			heap.blockInfo[index - 1].used = BLOCK_FULL;
		else
			heap.blockInfo[index - 1].used = BLOCK_SPLITTED;

	return returnBlock;
}

int findUsedBlock(int index) {
	if (index == 0)
		return 0;

	if (heap.blockInfo[index - 1].used == BLOCK_FULL) {
		return index;
	}

	return findUsedBlock(PARENT_BLOCK(index));
}

void mergeEmptySiblings(int index) {
	if (index == 1)
		return;

	if (heap.blockInfo[index - 1].used == BLOCK_EMPTY && heap.blockInfo[SIBLING_BLOCK(index) - 1].used == BLOCK_EMPTY) {
		heap.blockInfo[PARENT_BLOCK(index) - 1].used = BLOCK_EMPTY;
		mergeEmptySiblings(PARENT_BLOCK(index));
	}
	else {
		heap.blockInfo[PARENT_BLOCK(index) - 1].used = BLOCK_SPLITTED;
		mergeEmptySiblings(PARENT_BLOCK(index));
	}
}

uint64_t findUsedBlockSize(void * address) {
	if (address == NULL || address < baseAddress || address >= baseAddress + TOTAL_SIZE)
		return -1;

	int index = (address - baseAddress) / BLOCK_SIZE + (HEAP_SIZE / 2);
	index = findUsedBlock(index + 1);
	if (index == 0)
		return -1;

	return heap.blockInfo[index - 1].size;
}

#ifdef TEST
	void printHeap() {
		int i;
		int level = 1;
		int currentPositionInLevel = 0;
		int maxPositionInLevel = 1;

		for(i = 0; i < HEAP_SIZE; i++) {
			if (currentPositionInLevel == maxPositionInLevel) {
				level++;
				currentPositionInLevel = 0;
				maxPositionInLevel *= 2;
			}

			switch (heap.blockInfo[i].used) {
				case BLOCK_EMPTY:
					printf("%d: %p - EMPTY - size: 0x%x\n", i + 1, heap.blockInfo[i].address, (unsigned int)heap.blockInfo[i].size);
					break;
				case BLOCK_SPLITTED:
					printf("%d: %p - SPLITTED - size: 0x%x\n", i + 1, heap.blockInfo[i].address, (unsigned int)heap.blockInfo[i].size);
					break;
				case BLOCK_FULL:
					printf("%d: %p - FULL - size: 0x%x\n", i + 1, heap.blockInfo[i].address, (unsigned int)heap.blockInfo[i].size);
					break;
			}
		
			currentPositionInLevel++;
		}
	}
#endif