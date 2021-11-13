CC = gcc
CFLAGS = -Wall -g

all: af mygrep

mygrep: mygrep.o compregex.o afn.o afd.o pile.o file.o
	$(CC) $(CFLAGS) $^ -o $@

af: af.o afd.o afn.o pile.o file.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o *~ *.bak

lint:
	indent -kr -ts4 *.c
