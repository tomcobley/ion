CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -std=c99 -Werror -pedantic -DDEBUG
BUILD   = testreading
COMMON  = ../battery.c ../battery.h check.c check.h ../log.c ../log.h ../batterystate.h ../sys.c ../sys.h

.PHONY: all clean runtests

all: $(BUILD)

runtests: all
	./testreading
	make clean

testreading: testreading.c $(COMMON) 

clean:
	rm -f *.o
	rm -f testreading

