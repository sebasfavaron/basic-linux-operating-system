#include <stdio.h>
#include <stdlib.h>
#include "testlib.h"

void checkIsNotNull(void * pointer) {
	if (pointer == NULL) {
		fail("	Expected: a non null pointer, got: a null pointer\n");
	}
	else {
		ok();
	}
}

void checkAddressesAreNotEqual(void * anAddress, void * otherAddress) {
	if (anAddress == otherAddress) {
		fail("	Expected: different addresses, got: same addresses\n");
	}
	else {
		ok();
	}
}

void checkIsEqualOrGreaterThan(int aNumber, int otherNumber) {
	if (aNumber < otherNumber) {
		fail("  Expected: first greater or equal than second, got: first lower than second\n");
	}
	else {
		ok();
	}
}

void checkIsEqualTo(int aNumber, int otherNumber) {
	if (aNumber != otherNumber) {
		fail("  Expected: first equal to second, got: first different to second\n");
	}
	else {
		ok();
	}
}

void ok() {
	printf("	OK!\n");
}

void fail(char * withError) {
	putchar('\n');
	fprintf(stderr, "%s", withError);
}