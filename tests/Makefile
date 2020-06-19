CC      = gcc
CFLAGS  = -Wall -g -std=c99 -pedantic -DDEBUG
BUILD   = testreading testanalysis testwriting testwebhook
LIBS    = -lcurl
COMMON  = ../main.c check.c ../log.c ../sys.c

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

testwebhook: testwebhook.c ../curl.c ../config.c $(COMMON)
	$(CC) $(CFLAGS) testwebhook.c ../curl.c ../config.c $(COMMON) -o testwebhook $(LIBS)

clean:
	rm -f *.o *.out
	rm -f $(BUILD)
	cd ./temp && rm -f *
