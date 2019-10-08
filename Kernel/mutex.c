#include <mutex.h>
#include <scheduler.h>
#include <lib.h>
#include <terminal.h>

#define RIGHT_INDEX(i) (((i) == MAX_BLOCKED_MUTEX - 1) ? 0 : (i) + 1)

static Mutex mutexes[MAX_MUTEXES];

void initializeMutexes() {
	int i, j;

	for (i = 0; i < MAX_MUTEXES; i++) {
		Mutex * m = &mutexes[i];

		m->used = FALSE;
		m->pidLock = 0;
		m->pollIndex = 0;
		m->offerIndex = 0;
		m->mutex = 0; 
		m->cantBlocked = 0;

		for (j = 0; j < MAX_BLOCKED_MUTEX; j++) {
			m->blockedQueue[j] = -1;
		}
	}
}

int createMutex(char * mutexName) {
	int pid = getCurrentPid();
	int i;

	if (mutexName == NULL)
		return -1;

	for (i = 0; i < MAX_MUTEXES; i++) {
		if (mutexes[i].used == FALSE) {
			mutexes[i].used = TRUE;
			int len = strlen(mutexName);
			memcpy(mutexes[i].name, mutexName, len+1);
			mutexes[i].pidLock = 0;
			mutexes[i].pollIndex = 0;
			mutexes[i].offerIndex = 0;
			mutexes[i].mutex = 0;
			mutexes[i].cantBlocked = 0;
			mutexes[i].pidCreator = pid;
			return i;
		}
	}

	return -1;
}

int getMutex(char * mutexName) {
	int i;

	for (i = 0; i < MAX_MUTEXES; i++) {
		if (strcmp(mutexes[i].name, mutexName) == 0 && mutexes[i].used == TRUE) {
			return i;
		}
	}

	return createMutex(mutexName);
}

void closeMutex(int mutex) {
	if (mutex >= MAX_MUTEXES || mutexes[mutex].used == FALSE) 
		return;

	if (getCurrentPid() != mutexes[mutex].pidCreator)
		return;

	mutexes[mutex].used = FALSE;
}

void lockMutex(int mutex) {
	int lock;
	int pid;

	if (mutex >= MAX_MUTEXES || mutexes[mutex].used == FALSE) 
		return;

	lock = _testAndSet(&(mutexes[mutex].mutex));

	if (lock != ACQUIRED) {
		pid = getCurrentPid();
		blockProcessMutex(mutex, pid);
		_yield();
	}
	else {
		pid = getCurrentPid();
		mutexes[mutex].pidLock = pid;
	}

}

void freeMutex(int mutex) {
	if (mutex >= MAX_MUTEXES) 
		return;

	if (mutexes[mutex].cantBlocked > 0 || mutexes[mutex].used == FALSE) {
		int pid = getCurrentPid();
		if (mutexes[mutex].pidLock != pid)
			return;

		unblockProcessMutex(mutex);
	}
	else {
		mutexes[mutex].mutex = 0;
	}
}

void blockProcessMutex(int mutex, int pid) {
	Mutex * m = &mutexes[mutex];

	if (m->cantBlocked < MAX_BLOCKED_MUTEX) {
		m->cantBlocked++;
		changeProcessState(pid, BLOCKED);
		m->blockedQueue[m->offerIndex] = pid;
		m->offerIndex = RIGHT_INDEX(m->offerIndex);
	}

	return;
}

void unblockProcessMutex(int mutex) {
	Mutex * m = &mutexes[mutex]; 

	if (m->cantBlocked > 0) {
		m->cantBlocked--;
		changeProcessState(m->blockedQueue[m->pollIndex], READY);
		m->pidLock = m->blockedQueue[m->pollIndex];
		m->blockedQueue[m->pollIndex] = -1;
		m->pollIndex = RIGHT_INDEX(m->pollIndex);
	}

	return;
}