#ifndef STDIO_H
#define STDIO_H

void write(int fd, char* buffer, int size);
void read(int fd, char* buffer, int size);
void putChar(char c);
int getChar();
int printf(const char* format, ...);
int scanf(const char* format, ...);
int sscanf(const char* format, const char * str, ...);
int parseInt(char* string, int * value);
int parseDouble(char* string, double * value);
int readLine(char * buffer, int maxSize);
int getMutex(char * mutexName);
void closeMutex(int mutex);
void lockMutex(int mutex);
void freeMutex(int mutex);
int semCreate(char * semName, int value);
int semOpen(char * semName);
void semClose(int sem);
void semWait(int sem);
void semSignal(int sem);
int getpid();
int createprocess(void * entryPoint, char * description, int argc, char * argv[]);
int killprocess(int pid);
int run(char * filename, int argc, char * argv[]);
int thread_create(void * entryPoint, int argc, char * argv[]);
int thread_remove(int thread);
int thread_join(int thread);
int create_pipe(char * pipeName);
int get_pipe(char * pipeName);
void close_pipe(int pipe);
int read_pipe(int pipe, char * buffer, int bytes);
int write_pipe(int pipe, char * buffer, int bytes);

#endif
