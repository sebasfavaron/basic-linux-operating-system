include ../Makefile.inc

MODULE=0003-hello.bin
SOURCES=$(wildcard [^_]*.c)
LIBC=$(wildcard ../Libc/*.o)

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -I../Libc/include -T module.ld _loader.c $(SOURCES) $(LIBC) -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean print
