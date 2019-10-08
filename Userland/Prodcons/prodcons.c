#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PRODUCERS 10
#define MAX_CONSUMERS 10
#define MAX_ITEMS 100

#define RIGHT_INDEX(i) (((i) == MAX_ITEMS - 1) ? 0 : (i) + 1)

void producer();
void consumer();

int buffer[MAX_ITEMS];
int firstIndex;
int lastIndex;
int itemCount;

int producerPID[MAX_PRODUCERS];
int consumerPID[MAX_CONSUMERS];

int producers;
int consumers;

int itemID;

int semaphore1;
int semaphore2;

int main(int argc, char *argv[]) {
	firstIndex = 0;
	lastIndex = 0;
	itemCount = 0;
	producers = 0;
	consumers = 0;
	itemID = 1;
	semaphore1 = semCreate("prodcons-full", 0);
	semaphore2 = semCreate("prodcons-empty", 0);

	printInstructions();

	printf("prodcons:~$> ");

	int exit = 0;
	char cmd;
	while (!exit) {
		cmd = getchar();
		switch (cmd) {
			case '1': {
				if (producers < MAX_PRODUCERS) {
					producerPID[producers] = createprocess(&producer, "producer", producers + 1, 0);
					producers++;
				}
				break;
			}
			case '2': {
				if (consumers < MAX_CONSUMERS) {
					consumerPID[consumers] = createprocess(&consumer, "consumer", consumers + 1, 0);
					consumers++;
				}
				break;
			}
			case '3': {
				if (producers > 0) {
					producers--;
					killprocess(producerPID[producers]);
				}
				break;
			}
			case '4': {
				if (consumers > 0) {
					consumers--;
					killprocess(consumerPID[consumers]);
				}
				break;
			}
			case 'p': {
				run("ps", 0, 0);
				printf("\nprodcons:~$> ");
				break;
			}
			case 'i': {
				printInstructions();
				printf("prodcons:~$> ");
				break;
			}
			case 'x': {
				exit = 1;
				break;
			}
		}

		while (getchar() != '\n');
	}

	int i;
	for(i = 0; i < producers; i++)
		killprocess(producerPID[i]);
	for(i = 0; i < consumers; i++)
		killprocess(consumerPID[i]);

	semClose(semaphore1);
	semClose(semaphore2);

	return 0;
}

void producer(int id) {
	while (1) {
		if (itemCount < MAX_ITEMS) {
			int item = itemID;
			buffer[lastIndex] = itemID++;
			itemCount++;
			lastIndex = RIGHT_INDEX(lastIndex);
			printf("Producer %d produced item %d (items left = %d)\n", id, item, itemCount);
			semSignal(semaphore2);
		}
		else {
			semWait(semaphore1);
		}
	}
}

void consumer(int id) {
	while (1) {
		if (itemCount > 0) {
			int item = buffer[firstIndex];
			itemCount--;
			firstIndex = RIGHT_INDEX(firstIndex);
			printf("Consumer %d consumed item %d (items left = %d)\n", id, item, itemCount);
			semSignal(semaphore1);
		}
		else {
			semWait(semaphore2);
		}
	}
}

void printInstructions() {
	printf("\n- Prodcons Demo -\n\n\
	Press 1 to create a producer\n\
	Press 2 to create a consumer\n\
	Press 3 to delete a producer\n\
	Press 4 to delete a consumer\n\
	Press p to show process info (ps)\n\
	Press i to show commands\n\
	Press x to exit\n\
	Every command must be followed by an enter\n\n");
}