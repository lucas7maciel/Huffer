
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

void decompress(char* filename) {
    // Define os paths do arquivo de input e output
    char inputPath[100], outputPath[100];
    char *outputDir = "./outputs/";
    
    sprintf(inputPath, "%s%s", outputDir, filename);

    // Remove a extensao .huff do arquivo que vai ser descompactado
    if (strlen(outputPath) <= 5) {
        outputPath[0] = '\0';
    } else {
        outputPath[strlen(outputPath) - 5] = '\0';
    }

    FILE *input = fopen(inputPath, "rb");

    // Le os 2 primeiros bytes do header (lixo e tamanho da arvore)
    unsigned char firstByte;
    unsigned char secondByte;

    if (fread(&firstByte, sizeof(firstByte), 1, input) != 1) {
        printf("Erro ao ler primeiro byte do header\n");
        fclose(input);
        exit(1);
    }

    if (fread(&secondByte, sizeof(secondByte), 1, input) != 1) {
        printf("Erro ao ler segundo byte do header\n");
        fclose(input);
        exit(1);
    }

    // Pega o tamanho do lixo (3 primeiros bits de firstByte)
    unsigned int trash = 0;

    for (int i = 0; i < 3; i++) {
        trash = (trash << 1) | ((firstByte >> (7 - i)) & 1);
    }

    trash = firstByte >> 5;

    // Pega o tamanho da arvore (5 ultimos bits de firstbyte e todos de secondbyte)
    unsigned int treeSize = 0;

    for (int i = 3; i < 8; i++) {
        treeSize = (treeSize << 1) | ((firstByte >> (7 - i)) & 1);
    }

    // Extract all 8 bits of the second byte
    for (int i = 0; i < 8; i++) {
        treeSize = (treeSize << 1) | ((secondByte >> (7 - i)) & 1);
    }

    // Constrói a arvore que foi informada no header
    printf("Reconstruindo arvore\n");
    printf("- Tamanho da arvore: %i\n", treeSize);
    printf("- Tamanho do lixo: %i\n", trash);

    TreeNode* tree = buildFromHeader(NULL, &treeSize, input);
    //printTree(tree, 1);

    char *extension = readExtension(input);
    sprintf(outputPath, "%s%s.%s", outputDir, delExtension(filename), extension);

    // Printa a arvore
    printf("Printando\n");
    //printTree(tree, 1);

    // Faz a tradução do arquivo.huff baseado na árvore criada
    FILE *output = fopen(outputPath, "wb");
    
    translateHuff(tree, input, output, trash);
    
    fclose(output);
    fclose(input);

    printf("\nArquivo descompactado com sucesso!");
}