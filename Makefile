CC 	= gcc
CFLAGS  = -Wall -g -std=c99 -pedantic
LIBS 	= -lcurl
OBJS 	= curl.c sys.c log.c main.c config.c
TARGETS = main

.PHONY = all clean

all: $(TARGETS)

main: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o ion $(LIBS)

clean:
	rm -f *.o $(TARGETS) ion
	cd temp && rm -f *
