CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -std=c99 -Werror -pedantic -DDEBUG
BUILD   = testreading testanalysis testwriting
COMMON  = ../battery.c check.c ../log.c ../sys.c

.PHONY: all clean runtests

all: $(BUILD)

runtests: all
	./testreading
	./testanalysis
	./testwriting
testreading: testreading.c $(COMMON)

testanalysis: testanalysis.c $(COMMON)

testwriting: testwriting.c $(COMMON)

clean:
	rm -f *.o
	rm -f testreading
	rm -f testanalysis
	rm -f testwriting
	cd ./temp && rm -f *
