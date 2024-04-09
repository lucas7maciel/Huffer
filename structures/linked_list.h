#include <stdio.h>
#include <stdlib.h>

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct TreeNode TreeNode;

typedef struct Node {
    TreeNode* tree;
    struct Node *next;
} Node;

Node *createNode(TreeNode *treeNode);

void sortedInsert(Node **head, TreeNode *treeNode);

void printList(Node *head);

void freeList(Node *head);

#endif