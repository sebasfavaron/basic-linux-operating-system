#ifndef TESTLIB_H
#define TESTLIB_H

void checkIsNotNull(void * pointer);
void checkAddressesAreNotEqual(void * anAddress, void * otherAddress);
void checkIsEqualOrGreaterThan(int aNumber, int otherNumber);
void checkIsEqualTo(int aNumber, int otherNumber);
void ok();
void fail(char * withError);

#endif