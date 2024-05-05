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

// Conferir se o filename contém o caminho do arquivo
int isPath(const char *str) {
    if (strchr(str, '/') != NULL || strchr(str, '\\') != NULL) {
        return 1;
    }

    return 0;
}

char* getDir(char *str) {
    char *last_slash = strrchr(str, '/');
    char *last_backslash = strrchr(str, '\\');
    char *end = NULL;

    if (last_slash != NULL || last_backslash != NULL) {
        if (last_slash != NULL && last_backslash != NULL)
            end = (last_slash > last_backslash) ? last_slash : last_backslash;
        else
            end = (last_slash != NULL) ? last_slash : last_backslash;
        
        char *path = malloc(end - str + 2);
        strncpy(path, str, end - str + 1);
        path[end - str + 1] = '\0';
        printf("Path %s\n", path);
        return path;
    } else {
        return strdup("");
    }
}

char* getFilename(const char *str) {
    char *last_slash = strrchr(str, '/');
    char *last_backslash = strrchr(str, '\\');
    char *start = NULL;

    if (last_slash != NULL || last_backslash != NULL) {
        if (last_slash != NULL && last_backslash != NULL)
            start = (last_slash > last_backslash) ? last_slash + 1 : last_backslash + 1;
        else
            start = (last_slash != NULL) ? last_slash + 1 : last_backslash + 1;
        
        return strdup(start);
    } else {
        return strdup(str);
    }
}