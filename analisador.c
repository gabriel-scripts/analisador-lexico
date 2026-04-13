#include <stdio.h>

int main() {
    char c;
    int line = 1, word = 0, character = 0;

    printf("Digite um texto (pressione Ctrl+D para finalizar):\n");

    while ((c = getchar()) != EOF) {
        character++;

        if (c == '\n') {
            line++;
            word = 0; // Reset word count for new line
        } else if (c == ' ' || c == '\t') {
            if (word > 0) {
                word = 0; // Reset word count for new word
            }
        } else {
            if (word == 0) {
                word++; // Start counting a new word
            }
        }
    }

    printf("Linhas: %d\n", line);
    printf("Palavras: %d\n", word);
    printf("Caracteres: %d\n", character);

    return 0;
}