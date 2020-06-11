CC = gcc
CFLAGS = -Werror -g -std=c99 -pedantic

.PHONY = all clean

all: battery

battery: battery.c

clean:
	rm -f *.o battery
