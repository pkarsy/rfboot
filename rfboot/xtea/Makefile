
CC=gcc
CFLAGS=-I. -std=gnu99 -O2
DEPS = 

#%.o: %.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)

test: test.o xtea.o
	gcc -o test  xtea.o test.o -I.

clean:
	rm *.o test
