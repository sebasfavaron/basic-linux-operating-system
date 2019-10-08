#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <stdint.h>

void printRegisters(uint64_t reg, uint64_t value);

void exceptionDispatcher(int exception, uint64_t rip, uint64_t rsp);

void zeroDivisionHandler();

void overflowHandler();

void invalidOpCodeHandler();

#endif
