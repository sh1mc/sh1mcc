CFLAGS=-std=c11 -g3 -static -Wall -Wextra
.PHONY: test clean

SRCS=$(wildcard src/*.c)
OBJS=$(subst src,build,$(SRCS:.c=.o))

bin/sh1mcc: $(OBJS)
	$(CC) -o bin/sh1mcc $(OBJS) $(LDFLAGS)

build/%.o: src/%.c src/sh1mcc.h
	$(CC) $(CFLAGS) -c -o $@ $<

test: bin/sh1mcc
	./test/test.sh

clean:
	$(RM) -rf bin/* build/*
