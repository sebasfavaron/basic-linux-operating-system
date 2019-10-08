#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <strings.h>

#define BUFFER_SIZE 25*80+1000

extern int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int create_pipe(char * pipeName) {
	return int80(32, pipeName, 0, 0, 0, 0);
}

int get_pipe(char * pipeName) {
	return int80(33, pipeName, 0, 0, 0, 0);
}

void close_pipe(int pipe) {
	return int80(34, pipe, 0, 0, 0, 0);
}

int read_pipe(int pipe, char * buffer, int bytes) {
	return int80(35, pipe, buffer, bytes, 0, 0);
}

int write_pipe(int pipe, char * buffer, int bytes) {
	return int80(36, pipe, buffer, bytes, 0, 0);
}

int thread_create(void * entryPoint, int argc, char * argv[]) {
	return int80(29, entryPoint, argc, argv, 0, 0);
}

int thread_remove(int thread) {
	return int80(30, thread, 0, 0, 0, 0);
}

int thread_join(int thread) {
	return int80(31, thread, 0, 0, 0, 0);
}

int semCreate(char * semName, int value) {
	return int80(24, semName, value, 0, 0, 0);
}

int semOpen(char * semName) {
	return int80(25, semName, 0, 0, 0, 0);
}

void semClose(int sem) {
	int80(26, sem, 0, 0, 0, 0);
}

void semWait(int sem) {
	int80(27, sem, 0, 0, 0, 0);
}

void semSignal(int sem) {
	int80(28, sem, 0, 0, 0, 0);
}

int receiveIPC(char * pipeName, char * buffer, int bytes) {
	int80(20, pipeName, buffer, bytes, 0, 0);
} 

int sendIPC(char * pipeName, char * message, int bytes) {
	int80(21, pipeName, message, bytes, 0, 0);
}

int getMutex(char * mutexName) {
	int80(16, mutexName, 0, 0, 0, 0);
}

void closeMutex(int mutex) {
	int80(17, mutex, 0, 0, 0, 0);
}

void lockMutex(int mutex) {
	int80(18, mutex, 0, 0, 0, 0);
}

void freeMutex(int mutex) {
	int80(19, mutex, 0, 0, 0, 0);
}

int createprocess(void * entryPoint, char * description, int argc, char * argv[]) {
  	return int80(11, entryPoint, description, 0, argc, argv);
}

int getpid() {
  	return int80(14, 0, 0, 0, 0, 0);
}

int killprocess(int pid) {
  	return int80(15, pid, 0, 0, 0, 0);
}

void listprocess() {
	int80(13, 0, 0, 0, 0, 0);
}

int run(char * filename, int argc, char * argv[]) {
    return int80(7, filename, argc, argv, 0, 0);
}

void write(int fd, char* buffer, int size) {
	int80(1,fd,buffer,size, 0, 0);
}

void read(int fd, char* buffer, int size) {
	int80(0, fd, buffer, size, 0, 0);
}

void putchar(char c) {
	write(1, &c, 1);
}

char getchar() {
	char ch;
	do
		read(0, &ch, 1);
	while(ch == '\0');
	return ch;
}

int printf(const char* format, ...) {
	va_list args;
	va_start(args, format);
	int num, length, precision;
	int precFlag = 0;
	double fractionary;
	char number[30];
	char ch;
	char* string;

	while((ch = *(format++)) != '\0') {
		if(ch == '%') {
			if((ch = *(format++)) == '.') {
				format += parseInt(format, &precision);
				precFlag = 1;
				ch = *(format++);
			}
			switch(ch) {
				case 'd':
					num = va_arg(args, int);
					length = itoa(num, number, 10);
					if(precFlag == 1 && length < precision) {
						int diff = precision - length;
						while(diff--)
							putchar('0');
					}
					write(1, number, precFlag == 1 && length > precision ? precision : length);
					break;
				case 'f':
					fractionary = va_arg(args, double);
					length = ftoa(fractionary, number, precFlag == 1? precision : 6);
					write(1, number, length);
					break;
				case 'c':
					putchar((char)va_arg(args, int));
					break;
				case 's':
					string = (char*)va_arg(args,char*);
					write(1,string,strlen(string));
					break;
				default:
					putchar(ch);
					break;
			}
			precFlag = 0;
		}
		else
			putchar(ch);
	}
	return 0;
}

int scanf(const char* format, ...) {
	va_list args;
	va_start(args, format);
	int read = 0;
	int index = 0;
	int length;
	char ch;
	char buffer[BUFFER_SIZE];
  	char * character;

  	length = readLine(buffer,BUFFER_SIZE);

	while((ch = *(format++)) != '\0') {
		if(ch == '%') {
			switch(ch = *(format++)) {
				case 'd':
          index += parseInt(&buffer[index], va_arg(args,int*));
					read++;
					break;
				case 'f':
	        index += parseDouble(&buffer[index], va_arg(args,double*));
					read++;
					break;
				case 'c':
        	character = va_arg(args, char*);
          *character = buffer[index++];
          read++;
          break;
				case 's':
          character = va_arg(args,char*);
          char aux;
          while( (aux = buffer[index++]) != '\0')
          	*(character++) = aux;
          *character = '\0';
          read++;
          break;
			}
		}
		if(ch != buffer[index++])
			return read;
  	}

	return read;
}

int sscanf(const char* format, const char* str, ...) {
	va_list args;
	va_start( args, format );
	int read = 0;
	int index;
	char ch;
  char* character;

	while((ch = *(format++)) != '\0') {
		if(ch == '%') {
			switch(ch = *(format++)) {
				case 'd':
        	index = parseInt(str, va_arg(args,int*));
          str += index;
					read++;
					break;
				case 'f':
	        index = parseDouble(str, va_arg(args,double*));
	        str += index;
					read++;
					break;
				case 'c':
          character = va_arg(args, char*);
          *character = *(str++);
          read++;
          break;
				case 's':
          character = va_arg(args,char*);
          char aux;
          while( (aux = *(str++)) != '\0')
          	*(character++) = aux;
          *character = '\0';
          read++;
          break;
			}
		}
		if(ch != *(str++))
			return read;
  }
	return read;
}


int parseInt(char* string, int * value) {
  *value = 0;
	int sign= 1;
	int digits = 0;
	char c;

  if(*string == '-') {
		if (isDigit(*(string + 1))) {
			sign = -1;
			digits++;
			string++;
		}
		else
			return 0;
	}

  while (isDigit(c = *string)){
		*value = (*value)*10+(c - '0');
    digits++;
    string++;
  }

  *value *= sign;

  return digits;
}

int parseDouble(char * string, double * value)
{
	*value = 0;
	int sign = 1;
	int dotFlag = 0;
	double decimalPlace = 10;
	int digits = 0;
	char c;

	if(*string == '-') {
		if (isDigit(*(string + 1))) {
			sign = -1;
			digits++;
			string++;
		}
		else
			return 0;
	}

  while (isDigit(c = *string) || (c == '.' && !dotFlag)) {
		if(c != '.' || dotFlag) {
			if(!dotFlag)
				*value = (*value)*10 + (c - '0');
			else {
				*value = *value + (c - '0')/decimalPlace;
				decimalPlace *= 10;
			}
			digits++;
			string++;
		}
		else {
			dotFlag = 1;
			digits++;
			string++;
		}
	}

  *value *= sign;

  return digits;

}

int readLine(char * buffer, int maxSize) {
	int length = 0;
	char c;

	while((c = getchar()) != '\n' && length < maxSize) {
		*(buffer++) = c;
		length++;
	}

	*buffer = '\0';
	return length;
}