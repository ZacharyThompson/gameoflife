CC = gcc
CFLAGS = -Wall -ggdb $(shell pkg-config --cflags raylib)
LDLIBS = $(shell pkg-config --libs raylib)
all:
	$(CC) $(CFLAGS) main.c -o gameoflife $(LDLIBS)
