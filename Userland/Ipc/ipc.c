#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESS 15

void sendProcess();
void receiveProcess();

static char * message;

static int processPID[MAX_PROCESS];
static int process;

int main(int argc, char *argv[]) {
	if (argc < 2 || strlen(argv[1]) > 15) {
		printf("Usage: ipc [message] (max. 15 chars without spaces)\n");
		return 0;
	}

	message = argv[1];
	process = 0;

	printInstructions();

	printf("ipc:~$> ");

	int exit = 0;
	char cmd;
	while (!exit) {
		cmd = getchar();
		switch (cmd) {
			case '1': {
				processPID[process++] = createprocess(&sendProcess, "ipc-process-1", 0, 0);
				break;
			}
			case '2': {
				processPID[process++] = createprocess(&receiveProcess, "ipc-process-2", 0, 0);
				break;
			}
			case 'p': {
				run("ps", 0, 0);
				printf("\nipc:~$> ");
				break;
			}
			case 'i': {
				printInstructions();
				printf("ipc:~$> ");
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
	for(i = 0; i < process; i++)
		killprocess(processPID[i]);

	return 0;
}

void sendProcess() {
	printf("Send process (pid = %d): started\n", getpid());
	printf("Send process (pid = %d): sending message\n", getpid());
	sendIPC("ipcdemo", message, strlen(message) + 1);
	printf("Send process (pid = %d): message (%s) sent\n", getpid(), message);
}

void receiveProcess() {
	char buffer[20];
	printf("Receive process (pid = %d): started\n", getpid());
	printf("Receive process (pid = %d): waiting for message\n", getpid());
	receiveIPC("ipcdemo", buffer, strlen(message) + 1);
	printf("Receive process (pid = %d): message (%s) received\n", getpid(), buffer);
}

void printInstructions() {
	printf("\n- IPC Demo -\n\n\
	Message to send = %s\n\n\
	Press 1 to create a process that sends the message\n\
	Press 2 to create a process that receives the message\n\
	Press p to show process info (ps)\n\
	Press i to show commands\n\
	Press x to exit\n\
	Every command must be followed by an enter\n\n", message);
}