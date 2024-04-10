
#include "linked_list.h"

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

//
typedef struct TreeNode {
    unsigned char value;
    int count;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Structure
TreeNode* createTreeNode(unsigned char value, int count);

TreeNode* buildTree(Node* list);

void freeTree(TreeNode* node);

// Header related
TreeNode* buildFromHeader(TreeNode* tree, int* size, FILE* file);

void preOrderTree(TreeNode* node, FILE* file);

// Coding and decoding
void getCode(TreeNode* tree, unsigned char character, int* code, int* size, int depth, unsigned int path);

void decodeHuff(TreeNode *tree, FILE *input, FILE *output, int trash);

// Aux functions
int getDepth(TreeNode* node);

int countNodes(TreeNode* node);

int countTrash(int* frequencies, int* codesSize);

int isLeaf(TreeNode* node);

void printTree(TreeNode* tree, int level);

#endif