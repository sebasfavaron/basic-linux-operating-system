#include <semaphore.h>
#include <mutex.h>
#include <scheduler.h>
#include <lib.h>
#include <terminal.h>

#define RIGHT_INDEX(i) (((i) == MAX_BLOCKED_SEM - 1) ? 0 : (i) + 1)

static Semaphore semaphores[MAX_SEMAPHORES]; 

void initializeSemaphores() {
	int i, j;

	for( i = 0 ; i < MAX_SEMAPHORES ; i++){
		Semaphore * sem = &semaphores[i];

		sem->used = FALSE;
		sem->pollIndex = 0;
		sem->offerIndex = 0;
		sem->cantBlocked = 0;
		char string[10] = "sem-";
		uintToBase(i+1, string+4, 10);
		sem->mutex = getMutex(string);
		sem->semaphore = 0;

		for (j = 0; j < MAX_BLOCKED_SEM; j++) {
			sem->blockedQueue[j] = -1;
		}
	}
}

int createSemaphore(char * semName, int value) {
	int pid = getCurrentPid();
	int i;

	if (semName == NULL)
		return -1;

	for (i = 0; i < MAX_SEMAPHORES; i++) {
		if (semaphores[i].used == FALSE) {
			semaphores[i].used = TRUE;
			int len = strlen(semName);
			memcpy(semaphores[i].name, semName, len+1);
			semaphores[i].pidCreator = pid;
			semaphores[i].pollIndex = 0;
			semaphores[i].offerIndex = 0;
			semaphores[i].cantBlocked = 0;
			semaphores[i].semaphore = value;
			return i;
		}
	}

	return -1;
}

int semCreate(char * semName, int value) {
	int index;

	index = semOpen(semName);

	if(index != -1)
		return -1;

	return createSemaphore(semName, value);
}

int semOpen(char * semName) {
	int i;

	for (i = 0; i < MAX_SEMAPHORES; i++) {
		if (strcmp(semaphores[i].name, semName) == 0 && semaphores[i].used == TRUE) {
			return i;
		}
	}

	return -1;
}

void semClose(int sem) {
	int pid; 

	if (sem >= MAX_SEMAPHORES || semaphores[sem].used == FALSE) 
		return;

	pid = getCurrentPid();

	if(pid != semaphores[sem].pidCreator)
		return;

	semaphores[sem].used = FALSE;
}

void semWait(int semaphore) {
	int pid;

	if (semaphore >= MAX_SEMAPHORES || semaphores[semaphore].used == FALSE) 
		return;

	Semaphore * sem = &semaphores[semaphore];

	lockMutex(sem->mutex);
	sem->semaphore--;

	if(sem->semaphore < 0) {
		pid = getCurrentPid();
		blockProcessSemaphore(semaphore, pid);
		freeMutex(sem->mutex);
		_yield();
	}

	freeMutex(sem->mutex);
}

void semSignal(int semaphore) {
	if (semaphore >= MAX_SEMAPHORES || semaphores[semaphore].used == FALSE)
	 	return;

	Semaphore * sem = &semaphores[semaphore];	

	lockMutex(sem->mutex);
	sem->semaphore++;

	if (sem->cantBlocked > 0) {
		unblockProcessSemaphore(semaphore);
		freeMutex(sem->mutex);
	}

	freeMutex(sem->mutex);
}

void blockProcessSemaphore(int sem, int pid) {
	Semaphore * s = &semaphores[sem];

	if (s->cantBlocked < MAX_BLOCKED_SEM) {
		s->cantBlocked++;
		changeProcessState(pid, BLOCKED);
		s->blockedQueue[s->offerIndex] = pid;
		s->offerIndex = RIGHT_INDEX(s->offerIndex);
	}

	return;
}

void unblockProcessSemaphore(int sem) {
	Semaphore * s = &semaphores[sem]; 

	if (s->cantBlocked > 0) {
		s->cantBlocked--;
		changeProcessState(s->blockedQueue[s->pollIndex], READY);
		s->blockedQueue[s->pollIndex] = -1;
		s->pollIndex = RIGHT_INDEX(s->pollIndex);
	}

	return;
}