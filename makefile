CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g

main: build/main.o build/doubly_circular_list.o build/sort.o build/interactive.o
	@mkdir -p build
	$(CC) $(CFLAGS) build/main.o build/doubly_circular_list.o build/sort.o build/interactive.o -o build/main

build/main.o: src/main.c include/interactive.h include/doubly_circular_list.h include/node.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o

build/doubly_circular_list.o: src/doubly_circular_list.c include/doubly_circular_list.h include/node.h include/sort.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c src/doubly_circular_list.c -o build/doubly_circular_list.o

build/sort.o: src/sort.c include/sort.h include/node.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c src/sort.c -o build/sort.o

build/interactive.o: src/interactive.c include/interactive.h include/doubly_circular_list.h include/node.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c src/interactive.c -o build/interactive.o

run: main
	./build/main

clean:
	rm -rf build

.PHONY: main run clean
