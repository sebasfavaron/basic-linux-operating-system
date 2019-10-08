#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#define MAX_SEMAPHORES 50
#define MAX_BLOCKED_SEM 20

#define TRUE 1
#define FALSE 0

typedef struct {
	int used;
    char name[20];
    int blockedQueue[MAX_BLOCKED_SEM];
    int pollIndex;
    int offerIndex;
    int cantBlocked;
    int mutex;
    int semaphore;
   	int pidCreator;
} Semaphore;

void initializeSemaphores();
int createSemaphore(char * semName, int value);
int getSemaphore(char * semName);
int semCreate(char * semName, int value);
int semOpen(char * semName);
void semClose(int sem);
void semWait(int semaphore);
void semSignal(int semaphore);
void blockProcessSemaphore(int sem, int pid);
void unblockProcessSemaphore(int sem);

#endif