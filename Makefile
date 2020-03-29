all:
	gcc -o todo -Wall -Wextra \
	src/*.c

clean:
	rm -f todo

install:
	cp todo /usr/bin
