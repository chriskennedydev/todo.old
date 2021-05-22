CC = gcc
CFLAGS = -Wall -Wfatal-errors -Wshadow -Wextra -O2
BINARY = todo
FILES = *.c
SRCDIR = src
INSTDIR = /usr/bin

all: clean todo

todo:
	$(CC) -o $(BINARY) $(SRCDIR)/$(FILES) $(CFLAGS)

clean:
	@rm -f $(BINARY)

install:
	mv $(BINARY) $(INSTDIR)
