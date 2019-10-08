#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: echo [args]\n");
		return 0;
	}
  	for(int i=1; i < argc; i++)
    	printf("%s ", argv[i]);
	putchar('\n');
	return 0;
}
