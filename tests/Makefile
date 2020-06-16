CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -std=c99 -Werror -pedantic
BUILD   = testreading
OBJS = sys.o log.o battery.o check.o

.SUFFIXES: .c .o

.PHONY: all clean runtests

all: $(BUILD)

runtests: 
	./testreading

log.o: ../log.c ../log.h

sys.o: ../sys.c ../sys.h

battery.o: ../battery.c ../battery.h

check.o: check.c check.h

testreading: sys.o log.o battery.o check.o testreading.c
	$(CC) -o testreading $(OBJS) $(CFLAGS) $(LIBS)

clean:
	rm -f $(wildcard *.o)
	rm -f testreading

