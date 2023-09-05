CC = gcc
CFLAGS = -Wall -Wextra -ggdb $(shell pkg-config --cflags raylib)
LDLIBS = $(shell pkg-config --libs raylib)

all:
	$(CC) $(CFLAGS) main.c -o gameoflife $(LDLIBS)

clean:
	rm -f gameoflife
