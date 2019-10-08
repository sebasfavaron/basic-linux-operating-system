
##Introduction:
This is an extension of the x64BareBones environment.

x64BareBones is a basic setup to develop operating systems for the Intel 64 bits architecture.

The final goal of the project is to provide an entry point for a kernel and the possibility to load extra binary modules separated from the main kernel.

##Features:
The modules added in this project provided:
  IPC (Inter Process Communication) using semaphores, Memory Management, Scheduling, Pipes and Threads.

Some extra challenges were the implementation of a working terminal and userland tools like ps, kill and basic string operations

More details are provided in 'Informe 1' and 'Informe 2'

#Environment setup:
##1- Install the following packages before building the Toolchain and Kernel:

nasm qemu gcc make

##2- Build the Toolchain

Execute the following commands on the x64BareBones project directory:

```$ cd Toolchain```
```$ make all```

##3- Build the Kernel

From the x64BareBones project directory run:

```$ make all```

##4- Run the kernel

From the x64BareBones project directory run:

```$ ./run.sh```


Authors: Diego Bruno, Oliver Balfour, Sebastian Favaron, Ximena Zuberbuhler