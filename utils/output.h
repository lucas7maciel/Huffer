#include "huffman_tree.h"
#include "input.h"

#ifndef OUTPUT_H
#define OUTPUT_H

// From Huffman Tree header
typedef struct TreeNode TreeNode;

// From Input header

//void setHeader(TreeNode *tree, FILE *output);
void printExtension(FILE *file, char *filename);

char* readExtension(FILE *file);

void setHeader(TreeNode *tree, FILE *output, int trash, int treeSize);

void printBinary(int trash, int treeSize, FILE *file);

void preOrderTree(TreeNode* node, FILE* file);

//
void translateHuff(TreeNode *tree, FILE *input, FILE *output, int trash);
#endif