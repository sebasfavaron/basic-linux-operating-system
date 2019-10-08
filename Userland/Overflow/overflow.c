#include <stdint.h>
#include <stdlib.h>

extern void overflow();

int main(int argc, char *argv[]) {
  overflow();
  return 0;
}
