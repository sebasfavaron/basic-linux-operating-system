#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <process.h>

int addThread(Process * p, void * entryPoint, int argc, char * argv[]);
void switchToNextThread(Process * p);
void removeThread(Process * p, int thread);

#endif