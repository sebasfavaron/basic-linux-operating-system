#include <stdint.h>
#include <stdlib.h>

extern void invalid_opcode();

int main(int argc, char *argv[]) {
  invalid_opcode();
  return 0;
}
