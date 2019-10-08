#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <process.h>

typedef struct ProcessSlotS {
	struct ProcessSlotS * next;
	Process * process;
} ProcessSlot;

extern void _yield();

ProcessSlot * newProcessSlot(Process * process);
void addProcess(Process * process);
void removeProcess(int pid);
int eqProcess(Process * a, Process * b);
StackFrame * schedule();
Process * getProcessById(int pid);
int getCurrentPid();
Process * getCurrentProcess();
int getCurrentThread();
void changeProcessState(int pid, processState state);
void saveKernelStack(void * rsp);
void * getKernelStack();
void * getCurrentStack();
void * getCurrentEntryPoint();
void printProcesses();
char * getStateFromNumber(int state);

#endif