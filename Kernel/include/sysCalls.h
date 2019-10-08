#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

int sysRead(uint64_t fileDescriptor, uint64_t buffer, uint64_t size, uint64_t r8, uint64_t r9);

int sysWrite(uint64_t fileDescriptor, uint64_t buffer, uint64_t size, uint64_t r8, uint64_t r9);

int sysClear(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysSetTimeZone(uint64_t timeZone, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysGetTime(uint64_t hour, uint64_t minute, uint64_t seconds, uint64_t r8, uint64_t r9);

int sysGetDate(uint64_t day, uint64_t month, uint64_t year, uint64_t r8, uint64_t r9);

int sysEcho(uint64_t echoOn, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysRun(uint64_t filename, uint64_t argc, uint64_t argv, uint64_t r8, uint64_t r9);

int sysMalloc(uint64_t address, uint64_t size, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysDrawPixel(uint64_t x, uint64_t y, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysGetResolution(uint64_t winWidth, uint64_t winHeight, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysCreateProcess(uint64_t entryPoint, uint64_t description, uint64_t isForeground, uint64_t argc, uint64_t argv);

int sysExit(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysPs(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysGetPid(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysKillProcess(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysGetMutex(uint64_t mutexName, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysCloseMutex(uint64_t mutex, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysLockMutex(uint64_t mutex, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysFreeMutex(uint64_t mutex, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysReceive(uint64_t name, uint64_t buffer, uint64_t bytes, uint64_t r8, uint64_t r9);

int sysSend(uint64_t name, uint64_t buffer, uint64_t bytes, uint64_t r8, uint64_t r9);

int sysAllocBlock(uint64_t size, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysDeallocBlock(uint64_t address, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysCreateSemaphore(uint64_t semName, uint64_t value, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysOpenSemaphore(uint64_t semName, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysCloseSemaphore(uint64_t semaphore, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysWaitSemaphore(uint64_t semaphore, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysSignalSemaphore(uint64_t semaphore, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysCreateThread(uint64_t entryPoint, uint64_t argc, uint64_t argv, uint64_t r8, uint64_t r9);

int sysRemoveThread(uint64_t thread, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysJoinThread(uint64_t thread, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysCreatePipe(uint64_t pipeName, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysGetPipe(uint64_t pipeName, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysClosePipe(uint64_t pipe, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int sysReadPipe(uint64_t pipe, uint64_t buffer, uint64_t bytes, uint64_t r8, uint64_t r9);

int sysWritePipe(uint64_t pipe, uint64_t buffer, uint64_t bytes, uint64_t r8, uint64_t r9);

int sysCallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

void sysCallsSetup();

#endif
