#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Retorna uma string com a extensao do arquivo
// (caso tenha mais de uma, apenas a ultimas)
const char* getExtension(char* filename) {
    const char *dot = strrchr(filename, '.');

    if (!dot || dot == filename) {
        return "";
    }
    return dot + 1;
}

// Remove a extensao do arquivo
char* delExtension(char* filename) {
    char *retStr;
    char *lastExt;

    if (filename == NULL) return NULL;

    if ((retStr = malloc (strlen (filename) + 1)) == NULL) return NULL;

    strcpy (retStr, filename);
    lastExt = strrchr (retStr, '.');

    if (lastExt != NULL)
        *lastExt = '\0';
        
    return retStr;
}

int negToArray(int index) {
    if (index < 0) {
        return (index * (-1)) + 256;
    }

    return index;
}

int arrayToNeg(int index) {
    if (index > 255) {
        return (index * (-1)) + 256;
    }

    return index;
}