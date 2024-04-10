#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Especifique o filename para utilizar o programa\n");
        exit(0);
    }

    // Caso o arquivo seja .huff, descompactar; caso não, compactar
    if (!strcmp(getExtension(argv[1]), "huff")) {
        decompress(argv[1]);
    } else {
        compress(argv[1]);
    }

    return 0;
}