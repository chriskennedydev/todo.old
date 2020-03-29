all:
	gcc -o todo -g -Wall -Wextra \
	src/*.c

clean:
	rm -f todo

install:
	cp todo /usr/bin
