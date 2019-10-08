#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void infiniteThread(int thread);

int threads;
int print;
int threadJoin;
int threadFinish;

int main(int argc, char *argv[]) {
	threads = 0;
	print = 1;
	threadJoin = 0;
	threadFinish = 0;

	printInstructions();

	printf("I'm thread A\n");

	printf("threads:~$> ");

	int exit = 0;
	char cmd;
	while (!exit) {
		cmd = getchar();
		switch (cmd) {
			case '1': {
				if (thread_create(&infiniteThread, threads+1, 0) != -1)
					threads++;
				else
					printf("Error (max. threads per process = 5)\n");
				break;
			}
			case '2': {
				if (threads >= 2)
					threadJoin = threads;
				else
					printf("Error (need to create at least 2 new threads)\n");
				break;
			}
			case '3': {
				if (threads >= 1)
					threadFinish = threads;
				else
					printf("Error (thread A isn't finishable)\n");
				break;
			}
			case '4': {
				thread_remove(threads);
				if (threads >= 1)
					threads--;
				else
					printf("Error (thread A isn't removable)\n");
				break;
			}
			case '5': {
				if (print == 1) {
					print = 0;
					printf("Thread's prints deactivated\n");
				}
				else {
					print = 1;
					printf("Thread's prints activated\n");
				}
				break;
			}
			case 'p': {
				run("ps", 0, 0);
				printf("\nthreads:~$> ");
				break;
			}
			case 'i': {
				printInstructions();
				printf("threads:~$> ");
				break;
			}
			case 'x': {
				exit = 1;
				break;
			}
		}

		while (getchar() != '\n');
	}

	return 0;
}

void infiniteThread(int thread) {
	while (threadFinish != thread) {
		if (print)
			printf("I'm thread %c\n", thread + 'A');

		if (threadJoin - 1 == thread) {
			int aux = threadJoin;
			threadJoin = 0;
			thread_join(aux);

			threads--;
		}
	}

	threadFinish = 0;
}

void printInstructions() {
	printf("\n- Thread Demo -\n\n\
	Press 1 to create a thread\n\
	Press 2 to join the last two threads (first one created waits for the other to finish)\n\
	Press 3 to finish last thread\n\
	Press 4 to remove last thread\n\
	Press 5 to deactivate/activate thread's prints\n\
	Press p to show process info (ps)\n\
	Press i to show commands\n\
	Press x to exit\n\
	Every command must be followed by an enter\n\n");
}