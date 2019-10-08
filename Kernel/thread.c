#include <thread.h>
#include <buddyMemoryAlloc.h>
#include <process.h>
#include <scheduler.h>

int addThread(Process * p, void * entryPoint, int argc, char * argv[]) {
	if (p->totalThreads == MAX_THREADS)
		return -1;

	int index = getEmptyThread(p);

	p->threads[index] = (Thread *) allocBlock(BLOCK_SIZE * 2);
	Thread * t = p->threads[index];
	t->entryPoint = entryPoint;
	t->joinThread = -1;
	t->isFinished = 0;

	uint64_t stackAddress = (uint64_t)t + (uint64_t)findUsedBlockSize((void*)t);
	t->userStack = stackAddress;
	t->userStack = fillStackFrame(entryPoint, t->userStack, argc, backupArguments(p, argc, argv));

	p->totalThreads++;
	return index;
}

void switchToNextThread(Process * p) {
	int thread = p->activeThread + 1;
	if (thread == MAX_THREADS)
		thread = 0;

	while (p->threads[thread] == NULL || p->threads[thread]->joinThread != -1 || p->threads[thread]->isFinished == 1) {
		if (p->threads[thread]->joinThread != -1) {
			int joinThread = p->threads[thread]->joinThread;
			if (p->threads[joinThread]->isFinished == 1) {
				p->threads[thread]->joinThread = -1;
				removeThread(p, joinThread);
				break;
			}
		}

		thread++;
		if (thread == MAX_THREADS)
			thread = 0;
	}

	p->activeThread = thread;
}

void joinThread(Process * p, int thread1, int thread2) {
	if (p->threads[thread2] == NULL)
		return;

	p->threads[thread1]->joinThread = thread2;
	_yield();
}

void removeThread(Process * p, int thread) {
	if (thread == 0 || p->threads[thread] == NULL)
		return;

	if (thread == p->activeThread) {
		p->threads[thread]->isFinished = 1;
		_yield();
	}
	else {
		deallocBlock(p->threads[thread]);
		p->threads[thread] = NULL;
		p->totalThreads--;
	}
}

int getEmptyThread(Process * p) {
	int i;
	for (i = 0; i < MAX_THREADS; i++) {
		if (p->threads[i] == NULL)
			return i;
	}

	return -1;
}