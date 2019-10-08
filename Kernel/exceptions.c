//exceptions.c
#include <exceptions.h>
#include <lib.h>
#include <terminal.h>
#include <keyboard.h>
#include <MMU.h>
#include <video.h>

char * registers[] = {"RSP", "RAX", "RBX", "RCX", "RDX", "RBP", "RDI", "RSI", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15"};

void printRegisters(uint64_t reg, uint64_t value) {
	print(registers[reg]);
	print(": 0x");
	printHex(value);
	print("   ");
	if((reg % 4) == 0 && reg != 0 || reg == 15)
		printc('\n');
}

void zeroDivisionHandler() {
	print("Zero Division Exception Occurred (Exception 0)");
}

void overflowHandler() {
	print("Overflow Exception Occurred (Exception 4)");
}

void invalidOpCodeHandler() {
	print("Invalid Op Code Exception Occurred (Exception 6)");
}

void exceptionDispatcher(int exception, uint64_t rip, uint64_t rsp) {
	switch(exception) {
		case 0:
			zeroDivisionHandler();
			break;
		case 4:
			overflowHandler();
			break;
		case 6:
			invalidOpCodeHandler();
			break;
	}
	print("  RIP: 0x");
	printHex(rip);
	printc('\n');
	printRegisters(0,rsp);
}
