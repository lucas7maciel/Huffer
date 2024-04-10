
#include "extension.h"
#include "header.h"
#include "huffman_tree.h"

#ifndef MAIN_H
#define MAIN_H

void compress(char *filename);

void decompress(char *filename);

char* getExtension(char* filename);

char* readExtension(FILE *file);

void setHeader(TreeNode *tree, FILE *output, int trash, int treeSize);

#endif