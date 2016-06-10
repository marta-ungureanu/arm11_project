CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean


all: clock.o clock_implementation.o pin_manipulation.o
	$(CC) -o clock clock.o clock_implementation.o pin_manipulation.o

clock.o: clock.c clock_misc.h
	$(CC) $(CFLAGS) -c clock.c

clock_implementation.o: clock_implementation.c clock_misc.h
	$(CC) $(CFLAGS) -c clock_implementation.c

pin_manipulation.o: pin_manipulation.c clock_misc.h
	$(CC) $(CFLAGS) -c pin_manipulation.c

clean:
	rm -f $(wildcard *.o)
	rm -f all
