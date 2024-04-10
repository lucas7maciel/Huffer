#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "huffman_tree.h"
#include  "linked_list.h"

TreeNode* createTreeNode(unsigned char value, int count) {
    TreeNode* node = (TreeNode *) malloc(sizeof(TreeNode));

    node -> value = value;
    node -> count = count;
    node -> left = NULL;
    node -> right = NULL;

    return node;
}

// Pega os 2 menores valores e transforma em um no da arvore
TreeNode* buildTree(Node* list) {
    while (list != NULL && list -> next != NULL) {
        TreeNode* newParent = createTreeNode('*', 0);
        int sum = 0; // A soma sera adicionada no fim da funcao
        
        // Adiciona o menor valor ao node da esquerda
        newParent -> left = list -> tree;
        sum = sum + (list -> tree -> count);

        // Se tiver um segundo menor valor (node direito)
        if (list -> next != NULL) {
            list = list -> next;

            newParent -> right = list -> tree;
            sum = sum + (list -> tree -> count);
        }

        newParent -> count = sum;
        sortedInsert(&list, newParent);

        list = list -> next;
    }

    return list -> tree;
}

int isLeaf(TreeNode* node) {
    return node -> right == NULL && node -> left == NULL;
}

void getCode(TreeNode* tree, unsigned char character, int* code, int* size, int depth, unsigned int path) {
    if (tree == NULL) return;

    if (tree -> value != character || !isLeaf(tree)) {
        getCode(tree -> left, character, code, size, depth + 1, path << 1);              // 0 no final para esquerda
        getCode(tree -> right, character, code, size, depth + 1, (path << 1) | 1);       // 1 no final para direita
        return;
    }

    // Caso encontre o caracteres
    size[(int)character] = depth;
    //printf("%c (%i - %i): ", character, (int)character, depth);

    for (int i = 0; i < depth; i++) {
        // Conferir
        //printf("%i", (path >> depth - i - 1) & 1);
        code[i] = (path >> depth - i - 1) & 1;
    }

    //printf("\n");
}

int getDepth(TreeNode* node) {
    if (node == NULL) return 0;

    int leftDepth = getDepth(node -> left);
    int rightDepth = getDepth(node -> right);
    
    return (leftDepth > rightDepth) ? (leftDepth + 1) : (rightDepth + 1);
}

int countNodes(TreeNode* node) {
    if (node == NULL)
        return 0;
    else
        return 1 + countNodes(node -> left) + countNodes(node -> right);
}

int countTrash(int* frequencies, int* codesSize) {
    int originalBits = 0, compressedBits = 0;

    for (int i = 0; i < 256; i++) {
        if (frequencies[i]) {
            originalBits += frequencies[i] * 8;
            compressedBits += frequencies[i] * codesSize[i];
        }
    }

    //printf("Bits originalmente: %i\n", originalBits);
    //printf("Bits comprimidos: %i\n", compressedBits);

    return compressedBits % 8 == 0 ? 0 : 8 - (compressedBits % 8);
}

void printTree(TreeNode *tree, int level)
{
        if (tree == NULL) return;

        //for (int i = 0; i < level; i++)
                //printf(i == level - 1 ? "|-" : "  ");

        //printf("%c: %d\n", tree -> value, tree -> count);

        // Maneira alternativa (melhor para desenvolvimento)
        printf("%c (%i), (level %i)\n", tree -> value, (int)(tree -> value), level);

        printTree(tree -> left, level + 1);
        printTree(tree -> right, level + 1);
}

// Decompress
// Exemplo: **CB***FEDA
TreeNode* buildFromHeader(TreeNode* tree, int* size, FILE* file) {
    // Caso a quantidade de caracteres da arvore ja tenha sido percorrida
    if ((*size) <= 0) return tree;

    // Obtem o caractere atual
    unsigned char curr;

    if (fread(&curr, sizeof(curr), 1, file) != 1) {
        printf("Error reading character from the tree");
        exit(1);
    }

    // Testa os scapeds (comentar melhor)
    int scaped = 0;

    if (curr == '\\') {
        scaped = 1;

        // Le o proximo caractere
        if (fread(&curr, sizeof(curr), 1, file) != 1) {
            printf("Error reading character from the tree");
            exit(1);
        }
    }

    // Se o node é vazio (*), precisa se espalhar para a esquerda e direita
    // Caso tenha um valor, apenas necessita ser armazenado em sua posição
    tree = createTreeNode(curr, 0);

    if (curr == '*' && !scaped) {
        tree -> left = buildFromHeader(NULL, size, file);
        tree -> right = buildFromHeader(NULL, size, file);
    }

    (*size)--;
    return tree;
}

void translateHuff(TreeNode *tree, FILE *input, FILE *output, int trash) {
    unsigned char curr;
    TreeNode* currTree = tree;

    while (fread(&curr, sizeof(curr), 1, input)) { // Programar exceção de erro na leitura
        // Le o proximo caractere para conferir se o byte será inteiramente preenchido
        int nextChar = fgetc(input);
        int stop = nextChar != EOF ? 0 : trash;

        // Após isso, volta 1 byte no ponteiro para que não atrapalhe a leitura
        if (fseek(input, -1, SEEK_CUR) != 0) {
            printf("Erro ao voltar ponteiro");
            exit(1);
        }

        for (int i = 7; i >= stop; i--) {
            int bit = (curr >> i) & 1;
            //printf("%i", bit);

            if (bit == 1) {
                currTree = currTree -> right;
            } else {
                currTree = currTree -> left;
            }

            // Caso o nó da árvore não tenha filhos, significa que achamos um caractere
            if (currTree -> left == NULL && currTree -> right == NULL) {
                fprintf(output, "%c", currTree -> value);

                // Resetamos o valor da arvore para que ela volte a procurar a partir da raiz
                currTree = tree;
            }
        }

        //printf(" ");

        if (nextChar == EOF) break;
    }
}
