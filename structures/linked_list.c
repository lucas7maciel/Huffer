#include <stdlib.h>
#include <stdio.h>

#include "linked_list.h"
#include "huffman_tree.h"

Node *createNode(TreeNode *treeNode) {
    Node *n = (Node *)malloc(sizeof(Node));

    if (n == NULL)
    {
        printf("Erro ao alocar memoria para lista encadeada");
        exit(1);
    }

    n -> tree = treeNode;
    n -> next = NULL;

    return n;
}

// Revisar essa pra entender tudo
void sortedInsert(Node **head, TreeNode *treeNode) { 
    Node* newNode = createNode(treeNode);
    Node* current;

    if ((*head) == NULL || (*head) -> tree -> count >= newNode -> tree -> count) {
        newNode -> next = *head;
        *head = newNode;
    } else {
        current = *head;

        while (current -> next != NULL && current -> next -> tree -> count < newNode -> tree -> count) {
            current = current -> next;
        }

        newNode -> next = current -> next;
        current -> next = newNode;
    }
}

void printList(Node *head) {
    while (head != NULL) {
        printf("%c: %i\n", head -> tree -> value, head -> tree -> count);
        head = head -> next;
    }
}

void freeList(Node *head) {
    while (head != NULL)
    {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}