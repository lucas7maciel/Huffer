#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "huffman_tree.h"

// Essa funcao printa os 2 primeiros bytes do arquivo (Tamanho do lixo, Tamanho da arvore)
void printInfos(int trash, int treeSize, FILE *file) {
    unsigned char firstByte = 0, secondByte = 0;

    // Armazena o tamanho do lixo (max=7) nos 3 primeiros bits do primeiro byte
    // Armazena parte do tamanho da arvore nos ultimos 5 bits do primeiro byte
    firstByte = (trash << 5) | (treeSize >> 8);
    secondByte = treeSize & 0xFF;

    // Passa os bytes para o header do arquivo
    fprintf(file, "%c%c", firstByte, secondByte);
}

void preOrderTree(TreeNode* node, FILE* file) {
    if (node == NULL) return;

    if ((node -> value == '*' || node -> value == '\\') && isLeaf(node)) {
        // Printa o caractere de escape para que '*' e '\' possam ser lidos
        //printf("\\");
        fprintf(file, "\\");
    }

    if (node -> left == NULL && node -> right == NULL) {
        //printf("%c", (char)(node -> value));
        fprintf(file, "%c", node -> value);
    } else if (node -> left != NULL && node -> right != NULL) {
        //printf("*");
        fprintf(file, "*");
    } else {
        //printf("\n*Algo de errado aconteceu na construcao da arvore\n");
    }

    preOrderTree(node -> left, file);
    preOrderTree(node -> right, file);
}

void setHeader(TreeNode *tree, FILE *output, int trash, int treeSize) {
    printInfos(trash, treeSize, output);
    preOrderTree(tree, output);
}