all: example

build/untar.o: tinyuntar/untar.c
	mkdir -p build
	gcc -c -o build/untar.o tinyuntar/untar.c

example: build/untar.o test/main.c
	mkdir -p build
	gcc -g -I tinyuntar -o build/untar build/untar.o test/main.c -lm

clean:
	rm -fr build

