#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: kill [pids]\n");
		return 0;
	}
 	for (int i=1; i < argc; i++) {
 		int pid;
 		parseInt(argv[i], &pid);
    	killprocess(pid);
 	}
	return 0;
}