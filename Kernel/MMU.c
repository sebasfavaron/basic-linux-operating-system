#include <stdint.h>
#include <moduleLoader.h>
#include <lib.h>
#include <terminal.h>
#include <buddyMemoryAlloc.h>
#include <process.h>

static const uint64_t PageSize = 0x200000;
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

char * moduleNames[] = {"shell", "sampleDataModule", "sampleCodeModule", "hello", "help", "date", "time", "clear", "graph", "zero", "invalidOpCode", "overflow", "echo", "ps", "prodcons", "ipc", "kill", "while1", "threads", "pipes", 0};
void * moduleAddresses[] = {0x0A00000, 0x0C00000, 0x0E00000, 0x1000000, 0x1200000, 0x1400000, 0x1600000, 0x1800000, 0x1A00000, 0x1C00000, 0x1E00000, 0x2000000, 0x2200000, 0x2400000, 0x2600000, 0x2800000, 0x2A00000, 0x2C00000, 0x2E00000, 0x3000000};

void clearBSS(void * bssAddress, uint64_t bssSize)
{
  memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
  return (void*)(
    (uint64_t)&endOfKernel
    + PageSize * 8        //The size of the stack itself, 32KiB
    - sizeof(uint64_t)      //Begin at the top of the stack
  );
}

void * initializeKernelBinary()
{

  loadModules(&endOfKernelBinary, moduleAddresses);
  clearBSS(&bss, &endOfKernel - &bss);
  return getStackBase();
}

void * getEntryPointFromName(char * filename) {
  int i = 0;
  while(moduleNames[i] != 0) {
    if(strcmp(filename, moduleNames[i]) == 0) {
      return moduleAddresses[i];
    }
    i++;
  }
  return 0;
}
