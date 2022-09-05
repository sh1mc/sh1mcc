CFLAGS=-std=c11 -g -static -Wall -Wextra
.PHONY: test clean

SRCS=$(wildcard src/*.c)
OBJS=$(subst src,build,$(SRCS:.c=.o))

bin/sh1mcc: $(OBJS)
	$(CC) -o bin/sh1mcc $(OBJS) $(LDFLAGS)

$(OBJS): src/sh1mcc.h
	cd build && $(CC) -c ../src/$(basename $(@F)).c ../$<

test: bin/sh1mcc
	./test/test.sh

clean:
	$(RM) -rf bin/* build/*

