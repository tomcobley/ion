CC 	= gcc
CFLAGS  = -Wall -g -std=c99 -pedantic
LIBS 	= -lcurl
OBJS 	= curl.c sys.c log.c battery.c config.c
TARGETS = battery

.PHONY = all clean

all: $(TARGETS)

battery: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o battery $(LIBS)

clean:
	rm -f *.o $(TARGETS)
	cd temp && rm -f *

