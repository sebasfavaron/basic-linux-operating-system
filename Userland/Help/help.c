#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static const char * man =
"\n- List of Modules -\n\n\
ps: Lists all process information\n\
shell: Opens another shell\n\
kill [pids]...: Kills process sent as arguments\n\
ipc [message]: Demo of send and receive messages between process\n\
prodcons: Executes producer and consumer process\n\
while1: Creates a process with a while(1) inside (execute it in background to test kill)\n\
threads: Demo of creation, join and removal of threads inside a process\n\
pipes: Demo of read/write pipes between process\n\
time: Prints out the current time\n\
date: Prints out the current date\n\
clear: Wipes out the screen\n\
hello [args]...: Prints arguments\n\
sampleCodeModule: A sample module\n\
zero: Creates a zero division exception\n\
invalidOpCode: Creates an invalid Op Code exception\n\
overflow: Creates an overflow exception\n\
echo [args]...: Prints what you send as arguments\n\
graph: Graphs linear or parabolic functions\n\n\
Add & before a module to run it in background\n\n\
^C: Exits current process running in schedule (unless process is master shell)";

int main(int argc, char *argv[]) {
  printf("%s\n", man);
  return 0;
}
