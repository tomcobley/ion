CC = gcc
CFLAGS = -Werror -Wall -g -std=c99 -pedantic
LIBS = -lcurl
OBJS = curl.o sys.o log.o battery.o 

.PHONY = all clean

all: battery

log.o: log.c log.h

sys.o: sys.c sys.h

curl.o: curl.c curl.h

battery.o: battery.c battery.h

battery: battery.o curl.o sys.o log.o batterystate.h
	$(CC) -o battery $(OBJS) $(CFLAGS) $(LIBS)

clean:
	rm -f *.o battery
