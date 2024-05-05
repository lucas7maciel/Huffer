#ifndef EXTENSION_H
#define EXTENSION_H

char* getExtension(char* filename);

char* delExtension(char* filename);

void printExtension(FILE *file, char *filename);

char* readExtension(FILE *file);

int isPath(const char *str);

char* getDir(const char *str);

char* getFilename(char *str);


#endif