#include "huffman_tree.h"
#include "output.h"
#include "input.h"

#ifndef MAIN_H
#define MAIN_H

char* getExtension(char* filename);
void compress(char *filename);
void decompress(char *filename);

#endif