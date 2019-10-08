#include <stdio.h>
#include <stdlib.h>
#include <buddyMemoryAlloc.h>
#include <testlib.h>

void testInitBuddy();
void * testAlloc(unsigned long int size, char * block);
int testDealloc(void * address, char * blockName);

extern typeHeap heap;

int allocationSize = 4096;
int memoryToAllocate;
void * allocatedMemory = NULL;
void * anAllocation = NULL;
int deallocReturn;

void testAllocMemory();
void testTwoAllocations();
void testDeallocMemory();
void givenAMemoryManager();
void givenAMemoryAmount();
void givenAnAllocation();
void whenMemoryIsAllocated();
void whenMemoryIsDeallocated();
void thenSomeMemoryIsReturned();
void thenTheTwoAdressesAreDifferent();
void thenDeallocSuccess();
void thenBothDoNotOverlap();

int main() {
	printf("Test alloc memory:\n");
	testAllocMemory();

	printf("\nTest two allocations:\n");
	testTwoAllocations();

	printf("\nTest dealloc memory:\n");
	testDeallocMemory();
}

void testAllocMemory() {
	givenAMemoryManager();
	givenAMemoryAmount();

	whenMemoryIsAllocated();

	thenSomeMemoryIsReturned();
}

void testTwoAllocations() {
	givenAMemoryManager();
	givenAMemoryAmount();
	givenAnAllocation();

	whenMemoryIsAllocated();

	thenSomeMemoryIsReturned();
	thenTheTwoAdressesAreDifferent();
	thenBothDoNotOverlap();
}

void testDeallocMemory() {
	givenAMemoryManager();
	givenAMemoryAmount();

	whenMemoryIsAllocated();
	whenMemoryIsDeallocated();

	thenDeallocSuccess();
}

void givenAMemoryManager() {
	initializeBuddyManager();
}

void givenAMemoryAmount() {
	memoryToAllocate = allocationSize;
}

void givenAnAllocation() {
	anAllocation = allocBlock(memoryToAllocate);
}

void whenMemoryIsAllocated() {
	allocatedMemory = allocBlock(memoryToAllocate);
	putchar('\n');
	printHeap();
	putchar('\n');
}

void whenMemoryIsDeallocated() {
	deallocReturn = deallocBlock(allocatedMemory);
	putchar('\n');
	printHeap();
	putchar('\n');
}

void thenSomeMemoryIsReturned() {
	checkIsNotNull(allocatedMemory);
}

void thenTheTwoAdressesAreDifferent() {
	checkAddressesAreNotEqual(anAllocation, allocatedMemory);
}

void thenDeallocSuccess() {
	checkIsEqualTo(deallocReturn, 0);
}

void thenBothDoNotOverlap() {
	int distance = anAllocation - allocatedMemory;
	distance = abs(distance);

	checkIsEqualOrGreaterThan(distance, allocationSize);
}