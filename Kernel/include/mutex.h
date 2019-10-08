#ifndef MUTEX_H
#define MUTEX_H

#define MAX_MUTEXES 100
#define MAX_BLOCKED_MUTEX 20

#define TRUE 1
#define FALSE 0

#define ACQUIRED 1

typedef struct {
	int used;
    char name[20];
    int pidLock;
    int blockedQueue[MAX_BLOCKED_MUTEX];
    int pollIndex;
    int offerIndex;
    int mutex;
    int cantBlocked;
    int pidCreator;
} Mutex;

extern int _testAndSet(void * mutex);
void initializeMutexes();
int createMutex(char * mutexName);
int getMutex(char * mutexName);
void closeMutex(int index);
void lockMutex(int mutex);
void freeMutex(int mutex);
void blockProcessMutex(int mutex, int pid);
void unblockProcessMutex(int mutex);

#endif