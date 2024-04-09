
#include "linked_list.h"

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

// From input header
int negToArray(int index);

//
typedef struct TreeNode {
    unsigned char value;
    int count;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* createTreeNode(unsigned char value, int count);

TreeNode* buildTree(Node* list);

void getCode(TreeNode* tree, unsigned char character, int* code, int* size, int depth, unsigned int path);

int getDepth(TreeNode* node);

int countNodes(TreeNode* node);

int countTrash(int* frequencies, int* codesSize);

void preOrderTree(TreeNode* node, FILE* file);

void printTree(TreeNode* tree, int level);

TreeNode* buildFromHeader(TreeNode* tree, int* size, FILE* file);

#endif