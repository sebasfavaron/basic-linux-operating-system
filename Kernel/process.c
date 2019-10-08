#include <process.h>
#include <scheduler.h>
#include <buddyMemoryAlloc.h>
#include <terminal.h>
#include <sysCalls.h>
#include <thread.h>
#include <lib.h>

static int pid = 0;

int createProcess(void * entryPoint, char * description, int ppid, int isForeGround, int argc, char * argv[]) {
	Process * p = (Process *) allocBlock(BLOCK_SIZE);

	p->pid = pid++;
	p->ppid = ppid;
	p->isForeGround = isForeGround;
	p->state = READY;
	p->heapSize = 0;
	p->activeThread = 0;
	p->totalThreads = 0;

	int i;
	for (i = 0; i < MAX_THREADS; i++)
		p->threads[i] = NULL;

	uint64_t heapAddress = (uint64_t)p + (uint64_t) sizeof(Process);
	p->heap = heapAddress;

	p->description = backupDescription(p, description);
	addThread(p, entryPoint, argc, argv);
	addProcess(p);

	return p->pid;
}

StackFrame * fillStackFrame(void * entryPoint, StackFrame * userStack, int argc, char ** argv) {
	uint64_t stackAddress = (uint64_t) userStack - (uint64_t) sizeof(StackFrame);
	StackFrame * frame = stackAddress;
	frame->gs =		0x001;
	frame->fs =		0x002;
	frame->r15 =	0x003;
	frame->r14 =	0x004;
	frame->r13 =	0x005;
	frame->r12 =	0x006;
	frame->r11 =	0x007;
	frame->r10 =	0x008;
	frame->r9 =		0x009;
	frame->r8 =		0x00A;
	frame->rsi =	(uint64_t) argv;
	frame->rdi =	(uint64_t) argc; 
	frame->rbp =	0x00D;
	frame->rdx =	(uint64_t) entryPoint;
	frame->rcx =	0x00F;
	frame->rbx =	0x010;
	frame->rax =	0x011;
	frame->rip =	(uint64_t) &callProcess;
	frame->cs =		0x008;
	frame->eflags = 0x202;
	frame->rsp =	(uint64_t) &(frame->base);
	frame->ss = 	0x000;
	frame->base =	0x000;

	return frame;
}

void * malloc(Process * p, uint64_t request) {
  	uint64_t futureAddress = (uint64_t)p->heap + p->heapSize + request;
  	if(futureAddress >= (uint64_t)p + (uint64_t)findUsedBlockSize((void*)p))
    	return NULL;
 
  	uint64_t address = (uint64_t)p->heap + p->heapSize;
  	p->heapSize = p->heapSize + request;

  	return address;
}

void callProcess(int argc, char ** argv, void * entryPoint) {
	((int (*)(int, char **))(entryPoint))(argc, argv);
	Process * p = getCurrentProcess();
	int thread = getCurrentThread();
	if (thread == 0)
		sysExit(0, 0, 0, 0, 0);
	else {
		p->threads[thread]->isFinished = 1;
		_yield();
	}
}

char * backupDescription(Process * p, char * description) {
	int len = strlen(description);
    	
    char * temp = malloc(p, (len+1)*sizeof(char));
    if(temp == NULL) {
      	return NULL;
   	}

    memcpy(temp, description, (len+1)*sizeof(char));
    return temp;
}

char ** backupArguments(Process * p, int argc, char ** argv) {
	if (argc < 1 || argv == NULL)
		return NULL;

  	char ** temp = malloc(p, argc * sizeof(char **));
    if(temp == NULL) {
    	return NULL;
    }

    for(int i = 0; i < argc; i++) {
    	int len = strlen(argv[i]);
    	
      	char * temp2 = malloc(p, (len+1)*sizeof(char));
      	if(temp2 == NULL) {
      		return NULL;
      	}

      	memcpy(temp2, argv[i], (len+1)*sizeof(char));
      	temp[i] = temp2;
    }

    return temp;
} 