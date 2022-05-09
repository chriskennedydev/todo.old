CC = gcc
DEBUG_FLAGS = -Wall -Wfatal-errors -Wshadow -Wextra -O2
RELEASE_FLAGS = -Wall -Wfatal-errors -Wshadow -Wextra -O3
BINARY = todo
BINDIR = bin
PKGDIR = pkg
FILES = *.c
SRCDIR = src
INSTDIR = /usr/local/bin

all: clean debug 

debug: | $(BINDIR)
	$(CC) -o $(BINDIR)/$(BINARY) $(SRCDIR)/$(FILES) $(DEBUG_FLAGS)

$(BINDIR):
	mkdir $@

release: | $(PKGDIR)
	$(CC) -o $(PKGDIR)/$(BINARY) $(SRCDIR)/$(FILES) $(RELEASE_FLAGS)

$(PKGDIR):
	mkdir $@

clean:
	@rm -rf $(BINDIR) $(PKGDIR)

install:
	mv $(PKGDIR)/$(BINARY) $(INSTDIR)
