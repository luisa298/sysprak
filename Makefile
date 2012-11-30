CC = /usr/bin/clang
CFLAGS = -Wall -Werror -g -pedantic -std=c99 -D_POSIX_SOURCE
LDFLAGS = -lm

TARGETS = performConnection connector
OBJFILES = $(addsuffix .o, $(TARGETS))

all: $(OBJFILES) client

clean:
	rm -f $(OBJFILES) client

client: $(OBJFILES) main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $+

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
