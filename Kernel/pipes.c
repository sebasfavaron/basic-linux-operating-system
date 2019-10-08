#include <pipes.h>
#include <process.h>
#include <scheduler.h>
#include <mutex.h>
#include <semaphore.h>
#include <naiveConsole.h>

#define RIGHT_INDEX(i) (((i) == PIPE_LENGTH - 1) ? 0 : (i) + 1)

static Pipe pipes[MAX_PIPES];

void initializePipes() {
	int i, j;

	for (i = 0; i < MAX_PIPES; i++) {
		Pipe * p = &pipes[i];

		p->used = FALSE;
		p->readIndex = 0;
		p->writeIndex = 0;
		p->bytes = 0;
		for (j = 0; j < PIPE_LENGTH; j++)
			p->buffer[j] = 0;
		p->pidCreator = 0;
		char pipeString1[10] = "pipe-R-";
		uintToBase(i, pipeString1+7, 10);
		p->semaphore1 = semCreate(pipeString1, 0);
		char pipeString2[10] = "pipe-W-";
		uintToBase(i, pipeString2+7, 10);
		p->semaphore2 = semCreate(pipeString2, 0);
	}
}

int createPipe(char * pipeName) {
	int pid = getCurrentPid();
	int i;

	if (pipeName == NULL)
		return -1;

	for (i = 0; i < MAX_PIPES; i++) {
		if (pipes[i].used == FALSE) {
			pipes[i].used = TRUE;
			int len = strlen(pipeName);
			memcpy(pipes[i].name, pipeName, len+1);
			pipes[i].readIndex = 0;
			pipes[i].writeIndex = 0;
			pipes[i].bytes = 0;
			pipes[i].pidCreator = pid;
			return i;
		}
	}

	return -1;
}

int getPipe(char * pipeName) {
	int i;

	for (i = 0; i < MAX_PIPES; i++) {
		if (strcmp(pipes[i].name, pipeName) == 0 && pipes[i].used == TRUE) {
			return i;
		}
	}

	return createPipe(pipeName);
}

void closePipe(int index) {
	if (index >= MAX_PIPES) 
		return;

	if (getCurrentPid() != pipes[index].pidCreator)
		return;

	pipes[index].used = FALSE;
	semClose(pipes[index].semaphore1);
	semClose(pipes[index].semaphore2);
}

int pipeRead(int pipe, char * buffer, int bytes) {
	Pipe * p = &pipes[pipe];
	int i = 0;

	while (bytes != 0) {
		if (p->bytes == 0)
			semWait(p->semaphore1);
		else {
			buffer[i++] = p->buffer[p->readIndex];
			p->readIndex = RIGHT_INDEX(p->readIndex);
			p->bytes--;
			bytes--;
		}
	}

	semSignal(p->semaphore2);

	return 0;
}

int pipeWrite(int pipe, char * message, int bytes) {
	Pipe * p = &pipes[pipe];
	int i = 0;

	while (bytes != 0) {
		if (p->bytes == PIPE_LENGTH)
			semWait(p->semaphore2);
		else {
			p->buffer[p->writeIndex] = message[i++];
			p->writeIndex = RIGHT_INDEX(p->writeIndex);
			p->bytes++;
			bytes--;
		}
	}

	semSignal(p->semaphore1);

	return 0;
}