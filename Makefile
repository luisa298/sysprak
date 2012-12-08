CC = /usr/bin/gcc
CFLAGS = -Wall -Werror -g -pedantic -std=c99 -D_POSIX_SOURCE -D_SVID_SOURCE
LDFLAGS = -lm

TARGETS = performConnection connector service
OBJFILES = $(addsuffix .o, $(TARGETS))

all: $(OBJFILES) client

clean:
	rm -f $(OBJFILES) client

client: $(OBJFILES) main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $+

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
