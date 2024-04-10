#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Retorna uma string com a extensao do arquivo
// (caso tenha mais de uma, apenas a última)
char* getExtension(char* filename) {
    char *dot = strrchr(filename, '.');

    if (!dot || dot == filename) {
        return "";
    }

    return dot + 1;
}

// Retorna o filename com a extensao removida
char* delExtension(char* filename) {
    char *retStr;
    char *lastExt;

    if (filename == NULL) return NULL;

    if ((retStr = malloc(strlen(filename) + 1)) == NULL) return NULL;

    strcpy(retStr, filename);
    lastExt = strrchr(retStr, '.');

    if (lastExt != NULL)
        *lastExt = '\0';
        
    return retStr;
}

// Printa a extensão e seu tamanho no header do arquivo.huff
void printExtension(FILE *file, char *filename) {
    char *extension = getExtension(filename);

    // Armazena tamanho da etensao nos 3 primeiros bits
    unsigned char extSize = strlen(extension);
    extSize = extSize << 5;

    // Printa o byte que conterá o tamanho da extensão
    // em seus 3 primeiros bits
    fprintf(file, "%c", extSize);

    // Salva em forma de texto no arquivo
    for (int i = 0; i < strlen(extension); i++) {
        fprintf(file, "%c", extension[i]);
    }
}

// Lê a extensao salva no header
char* readExtension(FILE *file) {
    unsigned char curr;

    if (!fread(&curr, sizeof(curr), 1, file)) {
        printf("Falha ao ler extensao");
        exit(1);
    }

    // Move os bits para a direita, para que seu valor seja corrigido
    unsigned char extSize = curr >> 5;
    char *extension = (char*)malloc(extSize + 1);;

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

