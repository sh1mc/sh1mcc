CFLAGS=-std=c11 -g -static -Wall -Wextra
.PHONY: test

sh1mcc: sh1mcc.c

test: sh1mcc
	./test.sh

clean:
	rm -f sh1mcc *.o *~ tmp*

