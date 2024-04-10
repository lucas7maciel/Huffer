#ifndef EXTENSION_H
#define EXTENSION_H

typedef struct TreeNode TreeNode;

void printInfos(int trash, int treeSize, FILE *file);

void preOrderTree(TreeNode* node, FILE* file);

void setHeader(TreeNode *tree, FILE *output, int trash, int treeSize);

#endif
