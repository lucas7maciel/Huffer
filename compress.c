
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "main.h"

void compress(char *filename) {
    // Extrai a extensão e confere se ele tem 6 digitos
    // Caso tenha, não executa a compressão
    char *extension = getExtension(filename);

    if (strlen(extension) > 6) {
        printf("A compressao de arquivos com extensao acima de 6 caracteres nao e permitida");
        return;
    }

    // Define os paths do input e output
    char inputPath[100], outputPath[100];
    char *inputDir = "./inputs/";
    char *outputDir = "./outputs/";
    
    sprintf(inputPath, "%s%s", inputDir, filename);
    sprintf(outputPath, "%s%s.huff", outputDir, delExtension(filename));

    FILE *input = fopen(inputPath, "rb");

    if (input == NULL)
    {
        printf("Falha ao abrir arquivo, verifique se o mesmo existe");
        exit(1);
    }

    // Conta a frequencia dos caracteres
    int frequency[256] = {0}; // Consertar isso
    unsigned char current;
    
    while (fread(&current, sizeof(current), 1, input)) {
        frequency[(int)current]++;
    }

    printf("Arquivo lido\n");
    fclose(input);

    // Transforma em um lista encadeada de arvores ordenada pela frequencia da raiz
    Node *freqList = NULL;

    for (int i = 0; i < 256; i++) {
        if (!frequency[i]) continue;

        TreeNode* current = createTreeNode((unsigned char)i, frequency[i]);
        sortedInsert(&freqList, current);
    }

    // Construindo arvore
    printf("Construindo Arvore\n");
    TreeNode* huffmanTree = buildTree(freqList);
    printTree(huffmanTree, 0);

    // Cria dicionario para printar output
    // *Transformar em struct (pointer por byte)
    int treeDepth = getDepth(huffmanTree);
    int codes[256][treeDepth];
    int codesSize[256] = {0};

    for (int i = 0; i < 256; i++) {
        if (!frequency[i]) continue;

        getCode(huffmanTree, (unsigned char)i, codes[i], codesSize, 0, 0);
    }

    // Definir output
    FILE *output = fopen(outputPath, "wb"); // "Zera" arquivo, caso exista
    input = fopen(inputPath, "rb");

    // Define informações iniciais (tamanho do lixo, da árvore e árvore em pre ordem)
    int trash = countTrash(frequency, codesSize);
    int treeSize = countNodes(huffmanTree);

    // E passa as mesmas para o header do arquivo (2 bytes iniciais)
    printf("Setando Header\n");
    setHeader(huffmanTree, output, trash, treeSize);
    printExtension(output, filename);                   // Update: Extensão do arquivo no header

    // Informacoes
    printf("- Tamanho do lixo: %i\n", countTrash(frequency, codesSize));
    printf("- Tamanho da arvore: %i\n", countNodes(huffmanTree));

    // Salva as informações "traduzidas" do input no output
    printf("Escrevendo conteudo\n");

    unsigned char currByte = 0;
    int bitIndex = 0, bitsRemaining;

    // Enquanto tiverem caracteres a ser escritos
    while (fread(&current, sizeof(current), 1, input)) {
        // Quantia de bits usados no codigo do caractere atual
        int index = negToArray((int)current);
        bitsRemaining = codesSize[index];

        // Salva cada bit do codigo atual
        for (int i = 0; i < bitsRemaining; i++) {
            currByte = currByte << 1;

            if (codes[index][i]) {
                currByte |= 1;
            }

            ++bitIndex;

            // Caso um byte inteiro tenha sido preenchido, salva ele no output
            // E cria um segundo para completar a traducao
            if (bitIndex == 8) {
                fwrite(&currByte, sizeof(currByte), 1, output);

                bitIndex = 0;
                currByte = 0;
            }
        }
    }

    // Caso ainda restem bits a ser preenchidos
    if (bitIndex) {
        currByte = currByte << (8 - bitIndex);
        fwrite(&currByte, sizeof(currByte), 1, output);
    }

    fclose(output);

    // Liberar memoria da lista
    printf("Liberando memoria\n");
    freeList(freqList);

    //
    printf("\nArquivo comprimido com sucesso!");
}