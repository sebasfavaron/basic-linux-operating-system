#include <sysCalls.h>
#include <terminal.h>
#include <rtc.h>
#include <lib.h>
#include <MMU.h>
#include <scheduler.h>
#include <mutex.h>
#include <semaphore.h>
#include <pipes.h>
#include <buddyMemoryAlloc.h>
#include <thread.h>
#include <naiveConsole.h>

extern char * moduleNames[];
extern void * moduleAddresses[];

#define SYSCALLS 37

typedef int (*sys)(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

static sys sysCalls[SYSCALLS];

int sysRead(uint64_t fileDescriptor, uint64_t buffer, uint64_t size, uint64_t r8, uint64_t r9) {
	int index = 0;
	char c;
	if(fileDescriptor == 0) {
		while(index++ < size)
			*((char*)buffer++)= readBuffer();
	}
	return 0;
}

int sysWrite(uint64_t fileDescriptor, uint64_t buffer, uint64_t size, uint64_t r8, uint64_t r9) {
	if(fileDescriptor == 1 || fileDescriptor == 2) {
		char next;
		while(size--) {
			next = *(char *)(buffer++);
			if(fileDescriptor == 1)
				printc(next);
				else {
					color_t background = {0x00, 0x00, 0x00};
					color_t charColor = {0xFF, 0x00, 0x00};
					printcWithStyle(next, background, charColor);
				}
		}
	}
	return 0;
}

int sysClear(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	clearScreen();
	return 0;
}

int sysSetTimeZone(uint64_t timeZone, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	setTimeZone((int) timeZone);
	return 0;
}

int sysGetTime(uint64_t hour, uint64_t minute, uint64_t seconds, uint64_t r8, uint64_t r9) { /*Puede optimizarse*/
	*(int*)hour = getTime(HOURS);
	*(int*)minute = getTime(MINUTES);
	*(int*)seconds = getTime(SECONDS);
	return 0;
}

int sysGetDate(uint64_t day, uint64_t month, uint64_t year, uint64_t r8, uint64_t r9) {
	*(int*)day = getTime(DAY);
	*(int*)month = getTime(MONTH);
	*(int*)year = getTime(YEAR);
	return 0;
}

int sysEcho(uint64_t echoOn, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	setEcho((uint8_t) echoOn);
	return 0;
}

int sysRun(uint64_t filename, uint64_t argc, uint64_t argv, uint64_t r8, uint64_t r9) {
	int i = 0;
	int isForeground = 1;

	if (((char*)filename)[0] == '&') {
		isForeground = 0;
		filename = filename+1;
	}

	while(moduleNames[i] != 0) {
		if(strcmp(filename, moduleNames[i]) == 0) {
			Process * p = getCurrentProcess();
			int semaphore;
			
			if (isForeground) {
				char pidString[10] = "pid-";
				uintToBase(p->pid, pidString+4, 10);

				semaphore = semCreate(pidString, 0);
  			}

			int newpid = createProcess(moduleAddresses[i], filename, p->pid, isForeground, argc, argv);

			if (isForeground) {
				semWait(semaphore);
				semClose(semaphore);
			}

			return newpid;
		}

		i++;
	}
	return -1;
}

int sysMalloc(uint64_t address, uint64_t size, uint64_t rcx, uint64_t r8, uint64_t r9) {
	Process * p = getCurrentProcess();
	*((uint64_t *)address) = malloc(p, size);
	return 0;
}

int sysDrawPixel(uint64_t x, uint64_t y, uint64_t rcx, uint64_t r8, uint64_t r9) {
	drawPixel(x, y);
	return 0;
}

int sysGetResolution(uint64_t winWidth, uint64_t winHeight, uint64_t rcx, uint64_t r8, uint64_t r9) {
	*(int*)winWidth = width;
	*(int*)winHeight = height;
	return 0;
}

int sysCreateProcess(uint64_t entryPoint, uint64_t description, uint64_t isForeground, uint64_t argc, uint64_t argv) {
	int pid = getCurrentPid();

	int semaphore;
	if (isForeground) {
		char pidString[10] = "pid-";
		uintToBase(pid, pidString+4, 10);

		semaphore = semCreate(pidString, 0);
  	}

	int newpid = createProcess(entryPoint, description, pid, isForeground, argc, argv);

	if (isForeground) {
		semWait(semaphore);
		semClose(semaphore);
	}

	return newpid;
}

int sysExit(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	Process * p = getCurrentProcess();

	if (p->isForeGround && p->ppid != 0) {
		char ppidString[10] = "pid-"; 
		uintToBase(p->ppid, ppidString+4, 10);

		int semaphore = semOpen(ppidString);
  		semSignal(semaphore);
  	}

	removeProcess(p->pid);

	return 0;
}

int sysPs(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	printProcesses();
	return 0;
}

int sysGetPid(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	return getCurrentPid();
}

int sysKillProcess(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	Process * p = getProcessById(pid);

	if (p->isForeGround) {
		char ppidString[10] = "pid-"; 
		uintToBase(p->ppid, ppidString+4, 10);

		int semaphore = semOpen(ppidString);
  		semSignal(semaphore);
  	}

	removeProcess(p->pid);

	return 0;
}

int sysGetMutex(uint64_t mutexName, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	return getMutex(mutexName);
}

int sysCloseMutex(uint64_t mutex, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	closeMutex(mutex);
	return 0;
}

int sysLockMutex(uint64_t mutex, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	lockMutex(mutex);
	return 0;
}

int sysFreeMutex(uint64_t mutex, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	freeMutex(mutex);
	return 0;
}

int sysReceive(uint64_t name, uint64_t buffer, uint64_t bytes, uint64_t r8, uint64_t r9) {
	int pipe = getPipe(name);
	return pipeRead(pipe, buffer, bytes);
}

int sysSend(uint64_t name, uint64_t buffer, uint64_t bytes, uint64_t r8, uint64_t r9) {
	int pipe = getPipe(name);
	return pipeWrite(pipe, buffer, bytes);
}

int sysAllocBlock(uint64_t size, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	return allocBlock(size);
}

int sysDeallocBlock(uint64_t address, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	return deallocBlock(address);
}

int sysCreateSemaphore(uint64_t semName, uint64_t value, uint64_t rcx, uint64_t r8, uint64_t r9) {
	return semCreate(semName, value);
}

int sysOpenSemaphore(uint64_t semName, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	return semOpen(semName);
}

int sysCloseSemaphore(uint64_t semaphore, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	semClose(semaphore);
	return 0;
}

int sysWaitSemaphore(uint64_t semaphore, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	semWait(semaphore);
	return 0;
}

int sysSignalSemaphore(uint64_t semaphore, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	semSignal(semaphore);
	return 0;
}

int sysCreateThread(uint64_t entryPoint, uint64_t argc, uint64_t argv, uint64_t r8, uint64_t r9) {
	Process * p = getCurrentProcess();
	return addThread(p, entryPoint, argc, argv);
}

int sysRemoveThread(uint64_t thread, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	Process * p = getCurrentProcess();
	removeThread(p, thread);
	return 0;
}

int sysJoinThread(uint64_t thread, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	Process * p = getCurrentProcess();
	int t = getCurrentThread();

	if (thread > 0 && thread < MAX_THREADS) {
		joinThread(p, t, thread);
		return 0;
	}
	
	return -1;
}

int sysCreatePipe(uint64_t pipeName, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	return createPipe(pipeName);
}

int sysGetPipe(uint64_t pipeName, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	return getPipe(pipeName);
}

int sysClosePipe(uint64_t pipe, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	closePipe(pipe);
	return 0;
}

int sysReadPipe(uint64_t pipe, uint64_t buffer, uint64_t bytes, uint64_t r8, uint64_t r9) {
	return pipeRead(pipe, buffer, bytes);
}

int sysWritePipe(uint64_t pipe, uint64_t buffer, uint64_t bytes, uint64_t r8, uint64_t r9) {
	return pipeWrite(pipe, buffer, bytes);
}

int sysCallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	if(rdi < 0 || rdi >= SYSCALLS)
		return -1; //Tirar error??
	return sysCalls[rdi](rsi, rdx, rcx, r8, r9);
}

void sysCallsSetup() {
	sysCalls[0] = &sysRead;
	sysCalls[1] = &sysWrite;
	sysCalls[2] = &sysClear;
	sysCalls[3] = &sysSetTimeZone;
	sysCalls[4] = &sysGetTime;
	sysCalls[5] = &sysGetDate;
	sysCalls[6] = &sysEcho;
	sysCalls[7] = &sysRun;
	sysCalls[8] = &sysMalloc;
	sysCalls[9] = &sysDrawPixel;
	sysCalls[10] = &sysGetResolution;
	sysCalls[11] = &sysCreateProcess;
	sysCalls[12] = &sysExit;
	sysCalls[13] = &sysPs;
	sysCalls[14] = &sysGetPid;
	sysCalls[15] = &sysKillProcess;
	sysCalls[16] = &sysGetMutex;
	sysCalls[17] = &sysCloseMutex;
    sysCalls[18] = &sysLockMutex;
    sysCalls[19] = &sysFreeMutex;
    sysCalls[20] = &sysReceive;
    sysCalls[21] = &sysSend;
    sysCalls[22] = &sysAllocBlock;
    sysCalls[23] = &sysDeallocBlock;
    sysCalls[24] = &sysCreateSemaphore;
    sysCalls[25] = &sysOpenSemaphore;
    sysCalls[26] = &sysCloseSemaphore;
    sysCalls[27] = &sysWaitSemaphore;
    sysCalls[28] = &sysSignalSemaphore;
    sysCalls[29] = &sysCreateThread;
    sysCalls[30] = &sysRemoveThread;
    sysCalls[31] = &sysJoinThread;
    sysCalls[32] = &sysCreatePipe;
    sysCalls[33] = &sysGetPipe;
    sysCalls[34] = &sysClosePipe;
    sysCalls[35] = &sysReadPipe;
    sysCalls[36] = &sysWritePipe;
}
