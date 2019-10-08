#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
	if (argc < 2) {
		printf("Usage: hello [args]\n");
		return 0;
	}
	printf("Ave ");
 	for (int i=1; i < argc; i++)
    	printf("%s ", argv[i]);
	putchar('\n');
	return 0;
}
