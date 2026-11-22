# Компилятор и флаги
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g

# Главная цель: собрать программу из объектных файлов
main: build/main.o build/linked_list.o build/sort.o build/interactive.o
	@mkdir -p build
	$(CC) $(CFLAGS) build/main.o build/linked_list.o build/sort.o build/interactive.o -o build/main

# Как получить каждый объектный файл
build/main.o: src/main.c include/interactive.h include/linked_list.h include/node.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o

build/linked_list.o: src/linked_list.c include/linked_list.h include/node.h include/sort.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c src/linked_list.c -o build/linked_list.o

build/sort.o: src/sort.c include/sort.h include/node.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c src/sort.c -o build/sort.o

build/interactive.o: src/interactive.c include/interactive.h include/linked_list.h include/node.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c src/interactive.c -o build/interactive.o

# Собрать и запустить
run: main
	./build/main

# Удалить результаты сборки
clean:
	rm -rf build

.PHONY: main run clean
