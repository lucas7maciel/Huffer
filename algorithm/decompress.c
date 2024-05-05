#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

void decompress(char* filename) {
    // Define os paths do arquivo de input e output
    char *dir = isPath(filename) ? "" : "./files/";
    char inputPath[strlen(filename + 13)];
    char outputPath[strlen(filename + 21)];
    
    sprintf(inputPath, "%s%s", dir, filename);

    printf("Descomprimindo ");

    // Formatação: Negrito, Italico e Vermelho
    printf("\033[31m\033[3m\033[1m");
    printf("%s", getFilename(filename));
    printf("\033[0m\033[0m\033[0m...\n\n");

    FILE *input = fopen(inputPath, "rb");

    if (input == NULL) {
        printf("Falha ao abrir arquivo, verifique se o mesmo existe\n");
        exit(1);
    }

    // Lê o tamanho do lixo e o tamanho da árvore
    // *Lixo: 3 primeiros bits do primeiro byte do arquivo
    // *Árvore: Últimos 5 bits do primeiro byte e todos do segundo
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

    // Obtém o tamanho do lixo (3 primeiros bits do firstbyte)
    unsigned int trash = 0;

    for (int i = 0; i < 3; i++) {
        trash = (trash << 1) | ((firstByte >> (7 - i)) & 1);
    }

    trash = firstByte >> 5;

    // Obtém o tamanho da arvore (5 ultimos bits de firstByte e secondByte)
    // Unsigned int pois o tamanho da árvore pode conter mais de 8 bits
    // e um unsigned char pode conter apenas 8
    unsigned int treeSize = 0;

    // 5 ultimos bits de firstByte
    for (int i = 3; i < 8; i++) {
        treeSize = (treeSize << 1) | ((firstByte >> (7 - i)) & 1);
    }

    // Bits de secondByte
    for (int i = 0; i < 8; i++) {
        treeSize = (treeSize << 1) | ((secondByte >> (7 - i)) & 1);
    }

    // Exibe informações do header no terminal
    printf("- Arvore: \033[1m%i nodes\033[0m\n", treeSize);
    printf("- Lixo: \033[1m%i bits\033[0m\n", trash);

    // Com base no tamanho da árvore, a reconstrói com os caracteres
    // printados em pré ordem depois dos 2 primeiros bytes do header
    TreeNode* tree = buildFromHeader(NULL, &treeSize, input);

    // Depois de reconstruir a árvore, pegamos a extensão 
    // e atribuímos ao filename do output
    char *entension = readExtension(input);
    sprintf(outputPath, "%s%s.%s", dir, delExtension(filename), entension);

    // Faz a decodificação do arquivo.huff baseado na árvore criada
    FILE *output = fopen(outputPath, "wb");
    
    decodeHuff(tree, input, output, trash);
    freeTree(tree);

    fclose(output);
    fclose(input);

    // Confirma o fim do processo
    printf("- Arquivo: \033[1m\033[32m%s.%s\033[0m\033[0m\n\n", getFilename(delExtension(filename)), entension);
    printf("\033[1mArquivo descomprimido com sucesso!\033[0m\n");
}