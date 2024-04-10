CC=gcc
CFLAGS=-I./structures -I./utils -I./
DEPS=./structures/linked_list.h ./structures/huffman_tree.h ./utils/output.h ./utils/input.h
OBJ=./main.o ./compress.o ./decompress.o ./structures/linked_list.o ./structures/huffman_tree.o ./utils/output.o ./utils/input.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

huff: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) huff
