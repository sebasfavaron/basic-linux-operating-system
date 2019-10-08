include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: libc sampleCodeModule sampleDataModule shell hello help time date clear graph zero invalidOpCode overflow echo ps prodcons ipc kill while1 threads pipes

libc:
	cd Libc; make

sampleCodeModule:
	cd SampleCodeModule; make

sampleDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

shell:
	cd Shell; make

hello:
	cd Hello; make

help:
	cd Help; make

time:
	cd Time; make

date:
	cd Date; make

graph:
	cd Graph; make
	
zero:
	cd Zero; make

ps:
	cd Ps; make
		
prodcons:
	cd Prodcons; make

ipc:
	cd Ipc; make

kill:
	cd Kill; make
	
while1:
	cd While1; make

threads:
	cd Threads; make

pipes:
	cd Pipes; make

invalidOpCode:
	cd InvalidOpCode; make	

overflow:
	cd Overflow; make

echo:
	cd Echo; make

clear:
	cd Clear; make	

clean:
	cd SampleCodeModule; make clean
	cd Shell; make clean
	cd Libc; make clean
	rm -rf *.bin


.PHONY: sampleCodeModule all clean
