#ifndef _INTERRUPTIONS_H_
#define _INTERRUPTIONS_H_

#include <stdint.h>

#define INTGATE 0x8E
#define TRAPGATE 0x8F

void iSetHandler(int index, uint64_t handler, uint8_t attrs);
void sendEOI(int irq);
void setupIDT();
void irqDispatcher(int irq);
void tickHandlerC();

extern void exception0Handler();
extern void exception4Handler();
extern void exception6Handler();
extern void irq0Handler();
extern void tickHandler();
extern void startSchedule();
extern void irq1Handler();
extern void int80Handler();
extern void sti();
extern void cli();
extern void setPicMaster(uint8_t mask);
extern void setPicSlave(uint8_t mask);

#endif
