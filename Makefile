CFLAGS=-std=c99 -Wall -g -pedantic
LDFLAGS=-fsanitize=address -lcrypto -lssl
SOURCES := $(shell find $(SOURCEDIR) -name '*.c' | grep -v 'cmp_bit')
HEADERS  := $(shell find $(SOURCEDIR) -name '*.h')
OBJECTS=$(SOURCES:.c=.o)

all: steg

steg: $(OBJECTS)
	$(CC) -o steg $(LDFLAGS) $(OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(WFLAGS) -I./headers -c $< -o $@

clean:
	rm -f $(OBJECTS) steg

.PHONY: all clean steg