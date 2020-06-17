CC      = gcc
CFLAGS  = -Wall -g -std=c99 -pedantic -DDEBUG
BUILD   = testreading testanalysis testwriting testwebhook
COMMON  = ../battery.c check.c ../log.c ../sys.c ../curl.c

.PHONY: all clean runtests

all: $(BUILD)

runtests: all
	./testreading
	./testanalysis
	./testwriting
	./testwebhook
testreading: testreading.c $(COMMON)

testanalysis: testanalysis.c $(COMMON)

testwriting: testwriting.c $(COMMON)

testwebhook: testwebhook.c $(COMMON)

clean:
	rm -f *.o
	rm -f $(BUILD)
	cd ./temp && rm -f *
