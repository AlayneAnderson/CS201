# NOTE: All indented lines are indented with a tab character.

.PHONY: clean
CFLAGS= -ansi -pedantic -O0 -Wall -no-pie
DFLAGS = -g

all: hw3 hw3a1

hw3: hw3.o crc.o
	gcc -g -o hw3  $(CFLAGS) $(DFLAGS) hw3.o crc.o 
	
hw3a1: hw3.o hw3crc.o
	gcc  -g -o hw3a1 $(CFLAGS) $(DFLAGS) hw3.o hw3crc.o 

hw3.o: hw3.c
	gcc -c -g -o hw3.o $(CFLAGS) $(DFLAGS) hw3.c 

crc.o: crc.c
	gcc -c -g -o crc.o $(CFLAGS) $(DFLAGS) crc.c 

hw3crc.o: hw3crc.s
	as --64 -o hw3crc.o hw3crc.s

clean:
	rm -f *.o hw3 hw3a1

zip:
	zip rename_me.zip Makefile hw3.c crc.c hw3crc.s typescript
