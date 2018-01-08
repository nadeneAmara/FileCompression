FLAGS = -Wall -Wextra -Werror -pedantic
CC = gcc $(CFLAGS)

.PHONY: all
all:
	$(CC) -c bv.c
	$(CC) -c stack.c
	$(CC) -c queue.c	
	$(CC) -c huffman.c
	$(CC) -c encode.c
	$(CC) -c decode.c
	$(CC) -o encode encode.o bv.o stack.o queue.o huffman.o
	$(CC) -o decode decode.o bv.o stack.o queue.o huffman.o

bv.o	:	bv.c
	$(CC) -c bv.c
stack.o	:	stack.c
	$(CC) -c stack.c
queue.o	:	queue.c
	$(CC) -c queue.c
huffman.o	:	huffman.c
	$(CC) -c huffman.c
encode.o	:	encode.c
	$(CC) -c encode.c
encode	:	encode.o
	$(CC) -o encode.o
decode.o	:	decode.c
	$(CC) -c decode.c
decode	:	decode.o
	$(CC) -o decode.o

.PHONY: clean
clean:
	rm -f bv.o stack.o queue.o huffman.o encode.o encode decode.o decode
