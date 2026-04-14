#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Palavras reservadas
int qtd_kw_program = 0;
int qtd_kw_var = 0;
int qtd_kw_begin = 0;
int qtd_kw_end = 0;
int qtd_kw_if = 0;
int qtd_kw_then = 0;
int qtd_kw_else = 0;
int qtd_kw_while = 0;
int qtd_kw_do = 0;
int qtd_kw_integer = 0;
int qtd_kw_real = 0;

// Identificadores
int qtd_id = 0;

// Números
int qtd_num_int = 0;
int qtd_num_real = 0;

// Símbolos
int qtd_smb_sem = 0;   // ;
int qtd_smb_com = 0;   // ,
int qtd_smb_opa = 0;   // (
int qtd_smb_cpa = 0;   // )
int qtd_smb_col = 0;   // :
int qtd_smb_dot = 0;   // .
int qtd_smb_obc = 0;   // {
int qtd_smb_cbc = 0;   // }

// Operadores
int qtd_op_eq = 0;     // =
int qtd_op_neq = 0;    // <>
int qtd_op_lt = 0;     // <
int qtd_op_le = 0;     // <=
int qtd_op_gt = 0;     // >
int qtd_op_ge = 0;     // >=
int qtd_op_ass = 0;    // :=
int qtd_op_ad = 0;     // +
int qtd_op_min = 0;    // -
int qtd_op_mul = 0;    // *
int qtd_op_div = 0;    // /


struct Token {
    char nome[20]; 
    char tipo[20]; 
    char valor[50];
    char lexema[50];
    int linha;
    int coluna;
};

int validateToken(struct Token token) {

}

struct Token createToken(char *lexema) {
    struct Token token;

    char firstChar = lexema[0];

    if (strcmp(lexema, "program") == 0) {
        strcpy(token.nome, "KW_PROGRAM");
        strcpy(token.tipo, "palavra-chave");
        strcpy(token.valor, lexema);
        qtd_kw_program++;
    } else if (strcmp(lexema, "var") == 0) {
        strcpy(token.nome, "KW_VAR");
        strcpy(token.tipo, "palavra-chave");
        strcpy(token.valor, lexema);
        qtd_kw_var++;
    } else if (strcmp(lexema, "begin") == 0) {
        strcpy(token.nome, "KW_BEGIN");
        strcpy(token.tipo, "palavra-chave");
        strcpy(token.valor, lexema);
        qtd_kw_begin++;
    } else if (strcmp(lexema, "end") == 0) {
        strcpy(token.nome, "KW_END");
        strcpy(token.tipo, "palavra-chave");
        strcpy(token.valor, lexema);
        qtd_kw_end++;
    } else if (strcmp(lexema, "if") == 0) {
        strcpy(token.nome, "KW_IF");
        strcpy(token.tipo, "palavra-chave");
        strcpy(token.valor, lexema);
        qtd_kw_if++;
    } else if (strcmp(lexema, "then") == 0) {
        strcpy(token.nome, "KW_THEN");
        strcpy(token.tipo, "palavra-chave");
        strcpy(token.valor, lexema);
        qtd_kw_then++;
    } else if (strcmp(lexema, "else") == 0) {
        strcpy(token.nome, "KW_ELSE");
        strcpy(token.tipo, "palavra-chave");
        strcpy(token.valor, lexema);
        qtd_kw_else++;
    } else if (strcmp(lexema, "while") == 0) {
        strcpy(token.nome, "KW_WHILE");
        strcpy(token.tipo, "palavra-chave");
        strcpy(token.valor, lexema);
        qtd_kw_while++;
    } else if (strcmp(lexema, "do") == 0) {
        strcpy(token.nome, "KW_DO");
        strcpy(token.tipo, "palavra-chave");
        strcpy(token.valor, lexema);
        qtd_kw_do++;
    } else if (strcmp(lexema, "integer") == 0) {
        strcpy(token.nome, "KW_INTEGER");
        strcpy(token.tipo, "palavra-chave");
        strcpy(token.valor, lexema);
        qtd_kw_integer++;
    } else if (strcmp(lexema, "real") == 0) {
        strcpy(token.nome, "KW_REAL");
        strcpy(token.tipo, "palavra-chave");
        strcpy(token.valor, lexema);
        qtd_kw_real++;
    } else if (isdigit(firstChar)) {
        // número inteiro ou real
        if (strchr(lexema, '.') != NULL) {
            strcpy(token.nome, "NUM_REAL");
            strcpy(token.tipo, "número real");
            strcpy(token.valor, lexema);
            qtd_num_real++;
        } else {
            strcpy(token.nome, "NUM_INT");
            strcpy(token.tipo, "número inteiro");
            strcpy(token.valor, lexema);
            qtd_num_int++;
        }
    } else {
        strcpy(token.nome, "ID");
        strcpy(token.tipo, "identificador");
        strcpy(token.valor, lexema);
        qtd_id++;
    }

    return token;
}


int isToken(char *lexema){
    return 0;
}

// .err - arquivo de erros léxicos, .lex - arquivo de tokens, .ts - tabela de símbolos

void writeFiles(Token token){
    FILE *fp;


    if(token.tipo == "palavra-chave" || token.tipo == "identificador"){
        fp = fopen("result.lex", "w");

    } else if(token.tipo == "erro"){
        // Escreve no arquivo de err
    }else if(token.tipo == "simbolo" || token.tipo == "operador"){
        // Escreve no arquivo de simbolos
    }
}

void lexicalAnalisis(int c, FILE *fp){
    struct Token token;

    char lexema[50];
    int i = 0;

    int linha = 1;
    int coluna = 1;

    Token tokensResults[1000];

    while ((c = fgetc(fp)) != EOF) {

        printf("Caractere lido: %c\n", c);
        
        if (c == '\n') {
            linha++;
            coluna = 1;
        } else {
            coluna++;
        }

        if (isalnum(c)) { 
            lexema[i++] = c;
        } else if (isspace(c) || c == ';' || c == ':' || c == '(' || c == ')') {
            if (i > 0) {
                lexema[i] = '\0';
                struct Token token = createToken(lexema);
                printf("Token: %s, Valor: %s, Linha: %d, Coluna: %d\n", token.nome, token.valor, linha, coluna);
                i = 0;
            }
            
            if (c == ';') { printf("<SMB_SEM, ;>\n"); qtd_smb_sem++; }
            else if (c == ',') { printf("<SMB_COM, ,>\n"); qtd_smb_com++; }
            else if (c == '(') { printf("<SMB_OPA, (>\n"); qtd_smb_opa++; }
            else if (c == ')') { printf("<SMB_CPA, )>\n"); qtd_smb_cpa++; }
            else if (c == ':') { printf("<SMB_COL, :>\n"); qtd_smb_col++; }
            else if (c == '.') { printf("<SMB_DOT, .>\n"); qtd_smb_dot++; }
            else if (c == '{') { printf("<SMB_OBC, {>\n"); qtd_smb_obc++; }
            else if (c == '}') { printf("<SMB_CBC, }>\n"); qtd_smb_cbc++; }
            else if (c == '+') { printf("<OP_AD, +>\n"); qtd_op_ad++; }
            else if (c == '-') { printf("<OP_MIN, ->\n"); qtd_op_min++; }
            else if (c == '*') { printf("<OP_MUL, *>\n"); qtd_op_mul++; }
            else if (c == '/') { printf("<OP_DIV, />\n"); qtd_op_div++; }
            else if (c == '=') { printf("<OP_EQ, =>\n"); qtd_op_eq++; }

            tokensResults[i] = token; // armazena o token criado no array de resultados
            i++;

        } else {
            printf("Caractere inválido: %c\n", c);
        }
    }

}

int main() {
    FILE *fp;
    int c; 

    fp = fopen("codigo.pas", "r");
    if (fp == NULL) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    lexicalAnalisis(c, fp);

    fclose(fp);
    return 0;
}