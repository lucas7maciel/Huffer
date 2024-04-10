#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "output.h"

// Essa funcao printa os 2 primeiros bytes do arquivo (Tamanho do lixo, Tamanho da arvore)
void printBinary(int trash, int treeSize, FILE *file) {
    unsigned char firstByte = 0, secondByte = 0;

    // Armazena o tamanho do lixo (max=7) nos 3 primeiros bits do primeiro byte
    // Armazena parte do tamanho da arvore nos ultimos 5 bits do primeiro byte
    firstByte = (trash << 5) | (treeSize >> 8);
    secondByte = treeSize & 0xFF;

    // Printa informacoes
    //printf("First byte: %i\n", firstByte);
    //printf("Second byte: %i\n", secondByte);

    // Passa os bytes para o header do arquivo
    fprintf(file, "%c%c", firstByte, secondByte);
}

void printExtension(FILE *file, char *filename) {
    char *extension = getExtension(filename);

    // Armazena tamanho da etensao nos 3 primeiros bits
    unsigned char extSize = strlen(extension);
    extSize = extSize << 5;

    // Printa a extensao e armazena seu byte no header do arquivo
    fprintf(file, "%c", extSize);

    // Salva em forma de texto no arquivo
    for (int i = 0; i < strlen(extension); i++) {
        fprintf(file, "%c", extension[i]);
    }
}

// Le a extensao salva no header
char* readExtension(FILE *file) {
    unsigned char curr;

    if (!fread(&curr, sizeof(curr), 1, file)) {
        printf("Falha ao ler extensao");
        exit(1);
    }

    // Move os bits para a direita, para que seu valor seja corrigido
    unsigned char extSize = curr >> 5;
    char *extension = (char *)malloc(extSize + 1);;

    for (int i = 0; i < extSize; i++) {
        if (!fread(&curr, sizeof(curr), 1, file)) {
            printf("Falha ao ler extensao");
            exit(1);
        }

        extension[i] = curr;
    }

    extension[extSize] = '\0';

    return extension;
}

void setHeader(TreeNode *tree, FILE *output, int trash, int treeSize) {
    printBinary(trash, treeSize, output);
    preOrderTree(tree, output);
}

void translateHuff(TreeNode *tree, FILE *input, FILE *output, int trash) {
    unsigned char curr;
    TreeNode* currTree = tree;

    while (fread(&curr, sizeof(curr), 1, input)) { // Programar exceção de erro na leitura
        // Le o proximo caractere para conferir se o byte será inteiramente preenchido
        int nextChar = fgetc(input);
        int stop = nextChar != EOF ? 0 : trash;

        // Após isso, volta 1 byte no ponteiro para que não atrapalhe a leitura
        if (fseek(input, -1, SEEK_CUR) != 0) {
            printf("Erro ao voltar ponteiro");
            exit(1);
        }

        for (int i = 7; i >= stop; i--) {
            int bit = (curr >> i) & 1;
            //printf("%i", bit);

            if (bit == 1) {
                currTree = currTree -> right;
            } else {
                currTree = currTree -> left;
            }

            // Caso o nó da árvore não tenha filhos, significa que achamos um caractere
            if (currTree -> left == NULL && currTree -> right == NULL) {
                fprintf(output, "%c", currTree -> value);

                // Resetamos o valor da arvore para que ela volte a procurar a partir da raiz
                currTree = tree;
            }
        }

        //printf(" ");

        if (nextChar == EOF) break;
    }
}
