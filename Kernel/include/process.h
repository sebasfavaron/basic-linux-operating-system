#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define MAX_THREADS 5

typedef enum processState_t {RUNNING, READY, BLOCKED, DEAD} processState;

typedef struct {
	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	uint64_t rip;
	uint64_t cs;
	uint64_t eflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t base;
} StackFrame;

typedef struct {
	void * entryPoint;
	int joinThread;
	int isFinished;
	StackFrame * userStack;
} Thread;

typedef struct {
	int pid;
	int ppid;
	int isForeGround;
	processState state;
	char * description;

	int activeThread;
	int totalThreads;
	Thread * threads[MAX_THREADS];

	int heapSize;
	void * heap;
} Process;

int createProcess(void * entryPoint, char * description, int ppid, int isForeGround, int argc, char * argv[]);
StackFrame * fillStackFrame(void * entryPoint, StackFrame * userStack, int argc, char ** argv);
void * malloc(Process * p, uint64_t request);
void callProcess(int argc, char ** argv, void * entryPoint);
char * backupDescription(Process * p, char * description);
char ** backupArguments(Process * p, int argc, char ** argv);

#endif