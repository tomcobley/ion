CC = gcc
CFLAGS = -Werror -Wall -g -std=c99 -pedantic

.PHONY = all clean

all: battery

sys.o: sys.c sys.h

curl.o: curl.c curl.h # -lcurl

battery.o: battery.c battery.h

battery: battery.o curl.o sys.o 

clean:
	rm -f *.o battery
