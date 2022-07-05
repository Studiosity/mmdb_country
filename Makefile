PROG = mmdb_country
CC = gcc
CFLAGS = -Wall -Werror -Wmissing-prototypes
LDLIBS = -lmaxminddb

all: $(PROG)

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) $(@).c -o $@ $(LDLIBS)

clean:
	rm -f $(PROG)
