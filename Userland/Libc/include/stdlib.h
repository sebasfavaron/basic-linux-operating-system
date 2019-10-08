#ifndef STDLIB_H
#define STDLIB_H

void * malloc(unsigned int size);
int itoa(int value, char * buffer, int base);
int ftoa(float value, char * buffer, int precision);
int isAlpha(char c);
int isDigit(char c);
int exit();

#endif
