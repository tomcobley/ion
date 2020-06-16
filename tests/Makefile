CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -std=c99 -Werror -pedantic -DDEBUG
BUILD   = testreading testanalysis 
COMMON  = ../battery.c check.c ../log.c ../sys.c

.PHONY: all clean runtests

all: $(BUILD)

runtests: all
	./testreading
	./testanalysis 

testreading: testreading.c $(COMMON) 

testanalysis: testanalysis.c $(COMMON)

clean:
	rm -f *.o
	rm -f testreading
	rm -f testanalysis 

