#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    int c; 

    fp = fopen("codigo.pas", "r");
    if (fp == NULL) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    while ((c = fgetc(fp)) != EOF) {

        printf("Caractere lido: %c\n", c);

        // exemplo: verificar se é letra, dígito ou delimitador
        if (c >= 'a' && c <= 'z') {
            printf("É uma letra minúscula\n");
        } else if (c >= 'A' && c <= 'Z') {
            printf("É uma letra maiúscula\n");
        } else if (c >= '0' && c <= '9') {
            printf("É um dígito\n");
        } else if (c == ' ' || c == '\n' || c == '\t') {
            printf("É um delimitador\n");
        } else {
            printf("Outro símbolo\n");
        }
    }

    fclose(fp);
    return 0;
}