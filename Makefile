CC=gcc
CFLAGS=-I./algorithm/structures -I./algorithm/utils -I./algorithm
DEPS=./algorithm/structures/linked_list.h ./algorithm/structures/huffman_tree.h ./algorithm/utils/extension.h ./algorithm/utils/header.h
OBJ=./algorithm/main.o ./algorithm/compress.o ./algorithm/decompress.o ./algorithm/structures/linked_list.o ./algorithm/structures/huffman_tree.o ./algorithm/utils/extension.o ./algorithm/utils/header.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

huff: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) huff
