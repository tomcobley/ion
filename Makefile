CC 	= gcc
CFLAGS  = -Wall -g -std=c99 -pedantic
LIBS 	= -lcurl
OBJS 	= curl.c sys.c log.c main.c config.c
TARGETS = main

.PHONY = all clean

all: $(TARGETS)

main: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o main $(LIBS)

clean:
	rm -f *.o $(TARGETS)
	cd temp && rm -f *
