
#include "linked_list.h"

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#define EMPTY_CHAR 0xFF

// From input header
int negToArray(int index);

//
typedef struct TreeNode {
    int value;
    int count;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* createTreeNode(int value, int count);

TreeNode* buildTree(Node* list);

void getCode(TreeNode* tree, char character, int* code, int* size, int depth, unsigned char path);

int getDepth(TreeNode* node);

int countNodes(TreeNode* node);

int countTrash(int* frequencies, int* codesSize);

void preOrderTree(TreeNode* node, FILE* file);

void printTree(TreeNode* tree, int level);

TreeNode* buildFromHeader(TreeNode* tree, int* size, FILE* file);

#endif