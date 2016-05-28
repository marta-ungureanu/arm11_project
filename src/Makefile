CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: emulate #assemble emulate

assemble: assemble.o

emulate: emulate.o pipeline.o print.o multiply.o singleDataTransfer.o dataProcessing.o branch.o checkCondition.o
		$(CC) -o emulate emulate.o pipeline.o print.o multiply.o singleDataTransfer.o dataProcessing.o branch.o checkCondition.o

emulate.o: emulate.c
		$(CC) $(CFLAGS) -c emulate.c

pipeline.o: pipeline.c
		$(CC) $(CFLAGS) -c pipeline.c

print.o: print.c
		$(CC) $(CFLAGS) -c print.c

multiply.o: multiply.c
		$(CC) $(CFLAGS) -c multiply.c

singleDataTransfer.o: singleDataTransfer.c
		$(CC) $(CFLAGS) -c singleDataTransfer.c

dataProcessing.o: dataProcessing.c
		$(CC) $(CFLAGS) -c dataProcessing.c

branch.o: branch.c
		$(CC) $(CFLAGS) -c branch.c

checkCondition.o: checkCondition.c
		$(CC) $(CFLAGS) -c checkCondition.c





clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
