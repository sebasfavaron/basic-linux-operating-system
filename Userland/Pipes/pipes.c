#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESS 15

void process();

static int processPID[MAX_PROCESS];
static int processCount;

int main(int argc, char *argv[]) {
	processCount = 0;

	printInstructions();

	printf("pipes:~$> ");

	int exit = 0;
	char cmd;
	while (!exit) {
		scanf("%c", &cmd);
		switch (cmd) {
			case '1': {
				if (processCount >= 15) {
					printf("Max. process reached (15)");
					break;
				}

				char pipeName[20];
				printf("\nEnter pipe name (max. 19 chars): ");
				scanf("%s", &pipeName);
				if (strlen(pipeName) + 1 > 20) {
					printf("Invalid pipe name (max. 19 chars)");
					break;
				}

				char * args [2];
				args[0] = "read";
				args[1] = pipeName;
				processPID[processCount] = createprocess(&process, "pipes-process", 2, args);
				processCount++;
				break;
			}
			case '2': {
				if (processCount >= 15) {
					printf("Max. process reached (15)");
					break;
				}

				char pipeName[20];
				printf("\nEnter pipe name (max. 19 chars): ");
				scanf("%s", &pipeName);
				if (strlen(pipeName) + 1 > 20) {
					printf("Invalid pipe name (max. 19 chars)");
					break;
				}

				char message[15];
				printf("Enter message to write (max. 14 chars): ");
				scanf("%s", &message);
				if (strlen(message) + 1 > 15) {
					printf("Invalid message (max. 14 chars)");
					break;
				}

				char * args [3];
				args[0] = "write";
				args[1] = pipeName;
				args[2] = message;
				processPID[processCount] = createprocess(&process, "pipes-process", 3, args);
				processCount++;
				break;
			}
			case 'p': {
				run("ps", 0, 0);
				printf("\npipes:~$> ");
				break;
			}
			case 'i': {
				printInstructions();
				printf("pipes:~$> ");
				break;
			}
			case 'x': {
				exit = 1;
				break;
			}
		}
	}

	int i;
	for(i = 0; i < MAX_PROCESS; i++)
		killprocess(processPID[i]);

	return 0;
}

void process(int argc, char * argv[]) {
	if (strcmp(argv[0], "read") == 0) {
		char buffer[20];
		int pipe = get_pipe(argv[1]);
		printf("Read process (pid = %d, Pipe name = %s): started\n", getpid(), argv[1]);
		printf("Read process (pid = %d): reading 5 bytes from pipe\n", getpid());
		read_pipe(pipe, buffer, 5);
		printf("Read process (pid = %d): message (%s) read\n", getpid(), buffer);
		processCount--;
		return;
	}

	if (strcmp(argv[0], "write") == 0) {
		int pipe = get_pipe(argv[1]);
		printf("Write process (pid = %d, PipeName = %s): started\n", getpid(), argv[1]);
		printf("Write process (pid = %d): writing to pipe\n", getpid());
		write_pipe(pipe, argv[2], strlen(argv[2]) + 1) ;
		printf("Write process (pid = %d): message (%s) written\n", getpid(), argv[2]);
		processCount--;
		return;
	}
}

void printInstructions() {
	printf("\n- Pipes Demo -\n\n\
	Press 1 to create a process and read 5 bytes from a pipe\n\
	Press 2 to create a process and write a message to a pipe\n\
	Press p to show process info (ps)\n\
	Press i to show commands\n\
	Press x to exit\n\
	Every command must be followed by an enter\n\n");
}