#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "make_reg.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Especifique o filename para utilizar o programa\n");
        exit(0);
    }

    // Executado apenas uma vez para criar .reg (Windows)
    if (!strcmp(argv[1], "__build-reg")) {
        makeReg();
        return 0;
    }

    // Caso o arquivo seja .huff, descompactar; caso não, compactar
    if (!strcmp(getExtension(argv[1]), "huff")) {
        decompress(argv[1]);
    } else {
        compress(argv[1]);
    }

    return 0;
}