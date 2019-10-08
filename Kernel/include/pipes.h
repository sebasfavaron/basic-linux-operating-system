#ifndef PIPES_H
#define PIPES_H

#define PIPE_LENGTH 15
#define MAX_PIPES 5

#define TRUE 1
#define FALSE 0

#define NULL 0

typedef struct {
	int used;
	char name[20];
	int readIndex;
	int writeIndex;
	int bytes;
	int buffer[PIPE_LENGTH];
	int pidCreator;
	int semaphore1;
	int semaphore2;
} Pipe;

void initializePipes();
int createPipe(char * pipeName);
int getPipe(char * pipeName);
void closePipe(int index);
int pipeRead(int pipe, char * buffer, int bytes);
int pipeWrite(int pipe, char * message, int bytes);

#endif