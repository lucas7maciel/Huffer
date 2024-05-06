#include <stdio.h>
#include <unistd.h>

// Caso o OS utilizado seja um windows, cria-se uma .reg para que o programa possa ser executado pela
// barra de contexto
void createFile() {
    FILE *file = fopen("huffer.reg", "w");

    if (file == NULL) {
        printf("Não foi possível criar um arquivo .reg, mas a aplicação pode ser usada normalmente\n\n");
        return;
    }

    // Pegar o diretório do executável
    char path[1024];

    if (getcwd(path, sizeof(path)) == NULL) {
        printf("Não foi possível criar um arquivo .reg, mas a aplicação pode ser usada normalmente\n\n");
        return;
    }

    fprintf(file, "Windows Registry Editor Version 5.00\n\n");

    fprintf(file, "[HKEY_CLASSES_ROOT\\*\\shell\\Use Huffer]\n");
    fprintf(file, "@=\"Use Huffer\"\n\n");

    fprintf(file, "[HKEY_CLASSES_ROOT\\*\\shell\\Use Huffer\\command]\n");
    fprintf(file, "@=\"\\\"");

    // Printar o diretório
    for (int i = 0; path[i] != '\0'; i++) {
        // Arquivos .reg geralmente usam barras duplas para representar os diretórios
        if (path[i] == '\\') {
            fprintf(file, "\\\\");
        } else {
            fprintf(file, "%c", path[i]);
        }
    }

    fprintf(file, "\\\\huff\\\" \\\"%%1\\\"\"");
    fclose(file);
}

void makeReg() {
    #ifdef _WIN32
        createFile();
    #endif
}
