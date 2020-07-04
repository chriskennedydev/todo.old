all:
	gcc -o todo -Wall -Wextra -O2 \
	src/*.c

clean:
	rm -f todo

install:
	cp todo /usr/bin
