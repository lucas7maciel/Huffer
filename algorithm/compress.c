
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "main.h"

void compress(char *filename) {
    // Extrai a extensão e confere se ela tem 6 digitos ou mais
    // Caso tenha, não executa a compressão
    char *extension = getExtension(filename);

    if (strlen(extension) > 6) {
        printf("A compressao de arquivos com extensao acima de 6 caracteres nao e permitida\n");
        return;
    }

    printf("Comprimindo ");

    // Formatação: Negrito, Italico e Vermelho
    printf("\033[31m\033[3m\033[1m");
    printf("%s", getFilename(filename));
    printf("\033[0m\033[0m\033[0m...\n\n");

    // Define os paths do input e output
    char *dir = isPath(filename) ? "" : "./files/";
    char inputPath[strlen(filename) + 14];
    char outputPath[strlen(filename) + 18];

    sprintf(inputPath, "%s%s", dir, filename);
    sprintf(outputPath, "%s%s.huff", dir, delExtension(filename));

    FILE *input = fopen(inputPath, "rb");

    if (input == NULL) {
        printf("Falha ao abrir arquivo, verifique se o mesmo existe\n");
        exit(1);
    }

    // Conta a frequencia de cada caractere no arquivo
    int frequency[256] = {0};
    unsigned char current;
    
    while (fread(&current, sizeof(current), 1, input)) {
        frequency[(int)current]++;
    }

    fclose(input);

    // Transforma o array em uma lista encadeada, onde cada nó
    // da lista contém um nó de árvore que armazena seu valor (char)
    // e sua frequência
    Node *freqList = NULL;

    for (int i = 0; i < 256; i++) {
        if (!frequency[i]) continue;

        TreeNode* current = createTreeNode((unsigned char)i, frequency[i]);
        sortedInsert(&freqList, current);
    }

    // Constrói uma Árvore de Huffman com os valores da lista encadeada
    TreeNode* huffmanTree = buildTree(freqList);
    freeList(freqList);

    // Cria dicionario para printar output | *criar struct
    int treeDepth = getDepth(huffmanTree);
    int codes[256][treeDepth]; // *converter isso pra um unsigned int
    int codesSize[256] = {0};

    for (int i = 0; i < 256; i++) {
        if (!frequency[i]) continue;

        getCode(huffmanTree, (unsigned char)i, codes[i], codesSize, 0, 0);
    }

    // Iniciar a escrever os conteúdos do output
    FILE *output = fopen(outputPath, "wb");
    input = fopen(inputPath, "rb");

    // Informações da árvore para o header (Quantidade de nós e Tamanho do lixo)
    // *Lixo: Quantidade de bits sobrando após compressão
    int trash = countTrash(frequency, codesSize);
    int treeSize = countNodes(huffmanTree);

    // E passa as mesmas para o header do arquivo (Utilizando os 2 primeiros bytes)
    setHeader(huffmanTree, output, trash, treeSize);
    printExtension(output, filename); // Passa a extensão original do arquivo para o header
    freeTree(huffmanTree);            // Como não usaremos mais a árvore, liberamos a memória

    // Exibe informações no terminal
    printf("- Arvore: \033[1m%i nodes\033[0m\n", treeSize);
    printf("- Lixo: \033[1m%i bits\033[0m\n", trash);

    // Escreve os bytes codificados no output
    int bitIndex = 0, bitsRemaining;
    unsigned char currByte = 0;

    while (fread(&current, sizeof(current), 1, input)) {
        // Quantia de bits usados no codigo do caractere atual
        bitsRemaining = codesSize[(int)current];

        for (int i = 0; i < bitsRemaining; i++) {
            currByte = currByte << 1;

            if (codes[(int)current][i]) {
                currByte |= 1;
            }

            ++bitIndex;

            // Caso um byte inteiro tenha sido preenchido, salva ele no output
            // e cria outro para completar a traducao
            if (bitIndex == 8) {
                fwrite(&currByte, sizeof(currByte), 1, output);

                bitIndex = 0;
                currByte = 0;
            }
        }
    }

    // Caso ainda restem bits a ser preenchidos, movemos os que foram preenchidos
    // para o início do byte, assim não teremos problemas na hora de descomprimir
    if (bitIndex) {
        currByte = currByte << (8 - bitIndex);
        fwrite(&currByte, sizeof(currByte), 1, output);
    }

    fclose(output);
    
    // Confirma o fim do processo
    printf("- Arquivo: \033[1m\033[32m%s.huff\033[0m\033[0m\n\n", getFilename(delExtension(filename)));
    printf("\033[1mArquivo comprimido com sucesso!\033[0m\n");
}