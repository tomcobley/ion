CC = gcc
CFLAGS = -Werror -g -std=c99 -pedantic

.PHONY = all clean

all: battery

battery: battery.c curl.c curl.h -lcurl

clean:
	rm -f *.o battery
