#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
  printf("%.2d/%.2d/%.2d\n", getDay(), getMonth(), getYear());
  return 0;
}
