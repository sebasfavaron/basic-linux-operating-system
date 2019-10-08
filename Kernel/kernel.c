#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <interruptions.h>
#include <terminal.h>
#include <MMU.h>
#include <buddyMemoryAlloc.h>
#include <process.h>
#include <scheduler.h>
#include <mutex.h>
#include <semaphore.h>
#include <pipes.h>

#define PPID 0
#define NO_FOREGROUND 0
#define FOREGROUND 1
#define NO_ARGS 0

void init();

void init() {
	sysCreateProcess(getEntryPointFromName("shell"), "master shell", FOREGROUND, NO_ARGS, NO_ARGS);
	while(1);
}

int main()
{
	cli();
	clearScreen();
	setupIDT();
	initializeBuddyManager();
	initializeMutexes();
	initializeSemaphores();
	initializePipes();
	getResolution();

	createProcess(&init, "init", PPID, NO_FOREGROUND, NO_ARGS, NO_ARGS);
	startSchedule();
}
