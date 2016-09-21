CC=g++
CFLAGS=-c -Wall --std=c++14

all: bin/huffman

bin/main.o: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o bin/main.o

bin/huffman.o: src/huffman.cpp src/huffman.h
	$(CC) $(CFLAGS) src/huffman.cpp -o bin/huffman.o

bin/node.o: src/node.cpp
	$(CC) $(CFLAGS) src/node.cpp -o bin/node.o

bin/packer.o: src/packer.cpp
	$(CC) $(CFLAGS) src/packer.cpp -o bin/packer.o

bin/bitchar.o: src/bitchar.cpp
	$(CC) $(CFLAGS) src/bitchar.cpp -o bin/bitchar.o

bin/huffman: bin/main.o bin/huffman.o bin/node.o bin/packer.o bin/bitchar.o 
	$(CC) bin/main.o bin/huffman.o bin/node.o bin/packer.o bin/bitchar.o -o bin/huffman

test: bin/test

bin/test.o: test/test.cpp
	$(CC) $(CFLAGS) test/test.cpp -o bin/test.o

bin/huffman_test.o: test/huffman_test.cpp test/huffman_test.h
	$(CC) $(CFLAGS) test/huffman_test.cpp -o bin/huffman_test.o

bin/autotest.o: test/autotest.cpp test/autotest.h
	$(CC) $(CFLAGS) test/autotest.cpp -o bin/autotest.o

bin/test: bin/test.o bin/huffman_test.o bin/autotest.o bin/huffman.o bin/node.o bin/packer.o bin/bitchar.o
	$(CC) bin/test.o bin/huffman_test.o bin/autotest.o bin/huffman.o bin/node.o bin/packer.o bin/bitchar.o -o bin/test

clean:
	rm bin/*

.PHONY: clean

$(shell mkdir -p bin)
