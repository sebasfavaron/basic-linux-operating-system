/* sampleCodeModule.c */

#include <stdio.h>
#include <stdlib.h>

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;


int main(int argc, char *argv[]) {
	printf("This is a sample Module\n");
	return 0;
}
