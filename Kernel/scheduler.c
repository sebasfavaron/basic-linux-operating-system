#include <process.h>
#include <scheduler.h>
#include <buddyMemoryAlloc.h>
#include <terminal.h>
#include <MMU.h>
#include <thread.h>

static ProcessSlot * current = NULL;
static void * kernelStack = NULL;

static int cantProcesses = 0;

ProcessSlot * newProcessSlot(Process * process) {
	ProcessSlot * newProcessSlot = (ProcessSlot *) allocBlock(BLOCK_SIZE);
	newProcessSlot->process = process;
	return newProcessSlot;
}

void addProcess(Process * process) {
	ProcessSlot * newProcess = newProcessSlot(process);

	if (current == NULL) {
		current = newProcess;
		current->next = current;
	}
	else {
		ProcessSlot * prevToCurrent = current;
		while (prevToCurrent->next != current)
			prevToCurrent = prevToCurrent->next;
		prevToCurrent->next = newProcess;
		newProcess->next = current; 
	}

	cantProcesses++;
}

void removeProcess(int pid) {
	if (pid == 0 || pid == 1 || cantProcesses < 3) {
		//_yield();
		return;
	}

	ProcessSlot * prevSlot = current;
	ProcessSlot * slotToRemove = current->next;
	int i = 0;
	while (slotToRemove->process->pid != pid && i < cantProcesses) {
		prevSlot = slotToRemove;
		slotToRemove = slotToRemove->next;
		i++;
	}

	if (i == cantProcesses) {
		return;
	}

	if (eqProcess(slotToRemove->process, current->process)) {
		changeProcessState(slotToRemove->process->pid, DEAD);
		_yield();
	}
	else {
		int i;
		prevSlot->next = slotToRemove->next;
		cantProcesses--;
		for (i = 0; i < MAX_THREADS; i++) {
			Process * p = slotToRemove->process;
			deallocBlock(p->threads[i]);
		}
		deallocBlock(slotToRemove->process);
		deallocBlock(slotToRemove);
	}
}

int eqProcess(Process * a, Process * b) {
	return a->pid == b->pid;
}

StackFrame * schedule() {
	if (current->process->state == RUNNING) {
		current->process->state = READY;
	}

	switchToNextThread(current->process);

	current = current->next;
	while (current->process->state != READY) {
		if (current->process->state == DEAD) {
			int pid = current->process->pid;
			current = current->next;
			removeProcess(pid);
		}
		else
			current = current->next;
	}

	current->process->state = RUNNING;

	Process * p = current->process;

	return p->threads[p->activeThread]->userStack;
}

Process * getProcessById(int pid) {
	int i;
	ProcessSlot * slot = current;

	for (i=0; i < cantProcesses; i++) {
		if (slot->process->pid == pid)
			return slot->process;
		slot = slot->next;
	}

	return NULL;
}

int getCurrentPid() {
	return current == NULL ? -1 : current->process->pid;
}

Process * getCurrentProcess() {
	return current->process;
}

int getCurrentThread() {
	Process * p = current->process;
	return p->activeThread;
}

void changeProcessState(int pid, processState state) {
	int i;
 	ProcessSlot * slot = current;

	for (i = 0; i < cantProcesses; i++) {
		if (slot->process->pid == pid) {
			slot->process->state = state;
			return;
		}

		slot = slot->next;
	}

	return; 
}

void saveKernelStack(void * rsp) {
	kernelStack = rsp;
}

void * getKernelStack(void * rsp) {
	Process * p = current->process;
	p->threads[p->activeThread]->userStack = rsp;

	return kernelStack;
}

void * getCurrentStack() {
	Process * p = current->process;
	void * ret = p->threads[p->activeThread]->userStack;
	return ret;
}

void * getCurrentEntryPoint() {
	Process * p = current->process;
	void * ret = p->threads[p->activeThread]->entryPoint;
	return ret;
}

void printProcesses() {
	int i, j;
	ProcessSlot * s = current;
	printc('\n');
	for(i = 0; i < cantProcesses; i++) {
		Process * p = s->process;
		print("PID: ");
		printDec(p->pid);
		print("  |  PPID: ");
		printDec(p->ppid);
		print("  |  State: ");
		print(getStateFromNumber(p->state));
		print("  |  Description: ");
		print(p->description);
		print("  |  Heapsize: ");
		printDec(p->heapSize);
		print(" bytes");
		print("  |  Memory: 0x");
		printHex(p);
		print(" - 0x");
		uint64_t endAddress = (uint64_t)p + (uint64_t)findUsedBlockSize((void*)p) - 1;
		printHex(endAddress);
		for (j = 0; j < MAX_THREADS; j++) {
			if (p->threads[j] != NULL) {
				printc('\n');
				print("    Thread: ");
				printDec(j+1);
				if (p->activeThread == j)
					print(" (active)");
				print("  |  State: ");
				if (p->threads[j]->isFinished)
					print("FINISHED");
				else if (p->threads[j]->joinThread != -1) {
					print("JOIN (THREAD_");
					printDec(p->threads[j]->joinThread+1);
					print(")");
				}
				else
					print("RUNNING");
				print("  |  RSP: 0x");
				printHex(p->threads[j]->userStack);
				print("  |  Memory: 0x");
				printHex(p->threads[j]);
				print(" - 0x");
				uint64_t endAddress = (uint64_t)p->threads[j] + (uint64_t)findUsedBlockSize((void*)p->threads[j]) - 1;
				printHex(endAddress);
			}
		}
		printc('\n');
		s = s->next;
	}
}

char * getStateFromNumber(int state) {
	char * s;
	switch(state) {
			case 0: s="RUNNING";
					break;
			case 1:s="READY";
					break;
			case 2:s= "BLOCKED";
					break;
			case 3: s="DEAD";
					break;
			case 4: s="SLEEPING";
					break;
			default: s="other";
		}
		return s;
}