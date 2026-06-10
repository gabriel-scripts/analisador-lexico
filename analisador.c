#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "analisador.h"
#include "sintatico.h"

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

// Numeros
int qtd_num_int = 0;
int qtd_num_real = 0;

// Simbolos
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
int qtd_op_ne = 0;     // <>
int qtd_op_lt = 0;     // <
int qtd_op_le = 0;     // <=
int qtd_op_gt = 0;     // >
int qtd_op_ge = 0;     // >=
int qtd_op_ass = 0;    // :=
int qtd_op_ad = 0;     // +
int qtd_op_min = 0;    // -
int qtd_op_mul = 0;    // *
int qtd_op_div = 0;    // /

int qtd_erro = 0;

Simbolo tabela_ts[MAX_TS];
int qtd_ts = 0;

// Variaveis de controle de posicao
int linha = 1;
int coluna = 1;

void toLower(char *dst, const char *src, int max) {
    int i = 0;
    while (src[i] != '\0' && i < max - 1) {
        dst[i] = (char)tolower((unsigned char)src[i]);
        i++;
    }
    dst[i] = '\0';
}

int tsExiste(const char *lexema) {
    int i;
    for (i = 0; i < qtd_ts; i++) {
        if (strcmp(tabela_ts[i].lexema, lexema) == 0) {
            return 1;
        }
    }
    return 0;
}

void addTS(const char *lexema, const char *tipo) {
    if (qtd_ts >= MAX_TS || tsExiste(lexema)) {
        return;
    }
    strcpy(tabela_ts[qtd_ts].lexema, lexema);
    strcpy(tabela_ts[qtd_ts].tipo, tipo);
    qtd_ts++;
}

void initTS(void) {
    addTS("program", "palavra-reservada");
    addTS("var", "palavra-reservada");
    addTS("integer", "palavra-reservada");
    addTS("real", "palavra-reservada");
    addTS("begin", "palavra-reservada");
    addTS("end", "palavra-reservada");
    addTS("if", "palavra-reservada");
    addTS("then", "palavra-reservada");
    addTS("else", "palavra-reservada");
    addTS("while", "palavra-reservada");
    addTS("do", "palavra-reservada");
}

Token createSimpleToken(const char *nome, const char *tipo, const char *valor, int linha, int coluna) {
    Token token;
    memset(&token, 0, sizeof(Token));
    strcpy(token.nome, nome);
    strcpy(token.tipo, tipo);
    strcpy(token.valor, valor);
    token.linha = linha;
    token.coluna = coluna;
    return token;
}

void writeLex(FILE *fpLex, Token token) {
    fprintf(fpLex, "<%s, %s> %d %d\n", token.nome, token.valor, token.linha, token.coluna);
}

void writeErr(FILE *fpErr, const char *erro, const char *valor, int linha, int coluna) {
    fprintf(fpErr, "<%s, %s> %d %d\n", erro, valor, linha, coluna);
}

void writeTS(FILE *fpTs) {
    int i;
    for (i = 0; i < qtd_ts; i++) {
        fprintf(fpTs, "%s\t%s\n", tabela_ts[i].lexema, tabela_ts[i].tipo);
    }
}

Token createWordOrNumberToken(const char *lexemaOriginal, int linha, int coluna) {
    char lexema[MAX_LEXEMA];
    char firstChar;
    Token token;

    memset(&token, 0, sizeof(Token));
    toLower(lexema, lexemaOriginal, MAX_LEXEMA);
    firstChar = lexema[0];

    if (strcmp(lexema, "program") == 0) {
        qtd_kw_program++;
        return createSimpleToken("KW_PROGRAM", "palavra-chave", lexema, linha, coluna);
    }
    if (strcmp(lexema, "var") == 0) {
        qtd_kw_var++;
        return createSimpleToken("KW_VAR", "palavra-chave", lexema, linha, coluna);
    }
    if (strcmp(lexema, "begin") == 0) {
        qtd_kw_begin++;
        return createSimpleToken("KW_BEGIN", "palavra-chave", lexema, linha, coluna);
    }
    if (strcmp(lexema, "end") == 0) {
        qtd_kw_end++;
        return createSimpleToken("KW_END", "palavra-chave", lexema, linha, coluna);
    }
    if (strcmp(lexema, "if") == 0) {
        qtd_kw_if++;
        return createSimpleToken("KW_IF", "palavra-chave", lexema, linha, coluna);
    }
    if (strcmp(lexema, "then") == 0) {
        qtd_kw_then++;
        return createSimpleToken("KW_THEN", "palavra-chave", lexema, linha, coluna);
    }
    if (strcmp(lexema, "else") == 0) {
        qtd_kw_else++;
        return createSimpleToken("KW_ELSE", "palavra-chave", lexema, linha, coluna);
    }
    if (strcmp(lexema, "while") == 0) {
        qtd_kw_while++;
        return createSimpleToken("KW_WHILE", "palavra-chave", lexema, linha, coluna);
    }
    if (strcmp(lexema, "do") == 0) {
        qtd_kw_do++;
        return createSimpleToken("KW_DO", "palavra-chave", lexema, linha, coluna);
    }
    if (strcmp(lexema, "integer") == 0) {
        qtd_kw_integer++;
        return createSimpleToken("KW_INTEGER", "palavra-chave", lexema, linha, coluna);
    }
    if (strcmp(lexema, "real") == 0) {
        qtd_kw_real++;
        return createSimpleToken("KW_REAL", "palavra-chave", lexema, linha, coluna);
    }

    if (isdigit((unsigned char)firstChar)) {
        if (strchr(lexema, '.') != NULL) {
            qtd_num_real++;
            return createSimpleToken("NUM_REAL", "numero_real", lexema, linha, coluna);
        }
        qtd_num_int++;
        return createSimpleToken("NUM_INT", "numero_inteiro", lexema, linha, coluna);
    }

    qtd_id++;
    addTS(lexema, "identificador");
    return createSimpleToken("ID", "identificador", lexema, linha, coluna);
}

Token proximoToken(FILE *fp) {
    int c;
    char lexema[MAX_LEXEMA];

    while ((c = fgetc(fp)) != EOF) {
        int tokenLinha = linha;
        int tokenColuna = coluna;
        int i = 0;

        if (isspace(c)) {
            if (c == '\n') {
                linha++;
                coluna = 1;
            } else {
                coluna++;
            }
            continue;
        }

        if (c == '{') {
            int fechado = 0;
            while ((c = fgetc(fp)) != EOF) {
                if (c == '}') {
                    fechado = 1;
                    coluna++;
                    break;
                }
                if (c == '\n') {
                    linha++;
                    coluna = 1;
                } else {
                    coluna++;
                }
            }
            if (!fechado) {
                qtd_erro++;
                return createSimpleToken("ERROR", "ERRO_COMENTARIO_NAO_FECHADO", "{", tokenLinha, tokenColuna);
            }
            continue;
        }

        if (c == '\'') {
            int fechado = 0;
            coluna++;
            while ((c = fgetc(fp)) != EOF) {
                if (c == '\'') {
                    fechado = 1;
                    coluna++;
                    break;
                }
                if (c == '\n') {
                    qtd_erro++;
                    linha++;
                    coluna = 1;
                    return createSimpleToken("ERROR", "ERRO_STRING_NAO_FECHADA", "'", tokenLinha, tokenColuna);
                }
                coluna++;
            }
            if (c == EOF && !fechado) {
                qtd_erro++;
                return createSimpleToken("ERROR", "ERRO_STRING_NAO_FECHADA", "'", tokenLinha, tokenColuna);
            }
            continue;
        }

        if (isalpha(c)) {
            lexema[i++] = (char)c;
            coluna++;
            while ((c = fgetc(fp)) != EOF && isalnum(c)) {
                if (i < MAX_LEXEMA - 1) {
                    lexema[i++] = (char)c;
                }
                coluna++;
            }
            lexema[i] = '\0';
            if (c != EOF) {
                ungetc(c, fp);
            }
            return createWordOrNumberToken(lexema, tokenLinha, tokenColuna);
        }

        if (isdigit(c)) {
            int temPonto = 0;
            lexema[i++] = (char)c;
            coluna++;
            while ((c = fgetc(fp)) != EOF) {
                if (isdigit(c)) {
                    if (i < MAX_LEXEMA - 1) {
                        lexema[i++] = (char)c;
                    }
                    coluna++;
                } else if (c == '.' && !temPonto) {
                    int prox = fgetc(fp);
                    if (prox != EOF) {
                        ungetc(prox, fp);
                    }
                    if (isdigit(prox)) {
                        temPonto = 1;
                        if (i < MAX_LEXEMA - 1) {
                            lexema[i++] = (char)c;
                        }
                        coluna++;
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            }
            lexema[i] = '\0';
            if (c != EOF) {
                ungetc(c, fp);
            }
            return createWordOrNumberToken(lexema, tokenLinha, tokenColuna);
        }

        if (c == ';') {
            qtd_smb_sem++;
            coluna++;
            return createSimpleToken("SMB_SEM", "simbolo", ";", tokenLinha, tokenColuna);
        }
        if (c == ',') {
            qtd_smb_com++;
            coluna++;
            return createSimpleToken("SMB_COM", "simbolo", ",", tokenLinha, tokenColuna);
        }
        if (c == '(') {
            qtd_smb_opa++;
            coluna++;
            return createSimpleToken("SMB_OPA", "simbolo", "(", tokenLinha, tokenColuna);
        }
        if (c == ')') {
            qtd_smb_cpa++;
            coluna++;
            return createSimpleToken("SMB_CPA", "simbolo", ")", tokenLinha, tokenColuna);
        }
        if (c == '.') {
            qtd_smb_dot++;
            coluna++;
            return createSimpleToken("SMB_DOT", "simbolo", ".", tokenLinha, tokenColuna);
        }
        if (c == '+') {
            qtd_op_ad++;
            coluna++;
            return createSimpleToken("OP_AD", "operador", "+", tokenLinha, tokenColuna);
        }
        if (c == '-') {
            qtd_op_min++;
            coluna++;
            return createSimpleToken("OP_MIN", "operador", "-", tokenLinha, tokenColuna);
        }
        if (c == '*') {
            qtd_op_mul++;
            coluna++;
            return createSimpleToken("OP_MUL", "operador", "*", tokenLinha, tokenColuna);
        }
        if (c == '/') {
            qtd_op_div++;
            coluna++;
            return createSimpleToken("OP_DIV", "operador", "/", tokenLinha, tokenColuna);
        }
        if (c == '=') {
            qtd_op_eq++;
            coluna++;
            return createSimpleToken("OP_EQ", "operador", "=", tokenLinha, tokenColuna);
        }

        if (c == ':') {
            int prox = fgetc(fp);
            if (prox == '=') {
                qtd_op_ass++;
                coluna += 2;
                return createSimpleToken("OP_ASS", "operador", ":=", tokenLinha, tokenColuna);
            } else {
                if (prox != EOF) {
                    ungetc(prox, fp);
                }
                qtd_smb_col++;
                coluna++;
                return createSimpleToken("SMB_COL", "simbolo", ":", tokenLinha, tokenColuna);
            }
        }

        if (c == '<') {
            int prox = fgetc(fp);
            if (prox == '=') {
                qtd_op_le++;
                coluna += 2;
                return createSimpleToken("OP_LE", "operador", "<=", tokenLinha, tokenColuna);
            } else if (prox == '>') {
                qtd_op_ne++;
                coluna += 2;
                return createSimpleToken("OP_NE", "operador", "<>", tokenLinha, tokenColuna);
            } else {
                if (prox != EOF) {
                    ungetc(prox, fp);
                }
                qtd_op_lt++;
                coluna++;
                return createSimpleToken("OP_LT", "operador", "<", tokenLinha, tokenColuna);
            }
        }

        if (c == '>') {
            int prox = fgetc(fp);
            if (prox == '=') {
                qtd_op_ge++;
                coluna += 2;
                return createSimpleToken("OP_GE", "operador", ">=", tokenLinha, tokenColuna);
            } else {
                if (prox != EOF) {
                    ungetc(prox, fp);
                }
                qtd_op_gt++;
                coluna++;
                return createSimpleToken("OP_GT", "operador", ">", tokenLinha, tokenColuna);
            }
        }

        qtd_erro++;
        char invalido[2];
        invalido[0] = (char)c;
        invalido[1] = '\0';
        coluna++;
        return createSimpleToken("ERROR", "ERRO_CARACTERE_INVALIDO", invalido, tokenLinha, tokenColuna);
    }

    return createSimpleToken("EOF", "fim_de_arquivo", "EOF", linha, coluna);
}

void tv_free(TokenVec *v) {
    if (v->data) free(v->data);
    v->data = NULL;
    v->size = 0;
    v->cap = 0;
}

TokenVec tokenize_all(FILE *fp) {
    TokenVec v = {NULL, 0, 0};
    v.cap = 128;
    v.data = (Token*)malloc(v.cap * sizeof(Token));

    Token t;
    do {
        t = proximoToken(fp);
        if (v.size >= v.cap) {
            v.cap *= 2;
            v.data = (Token*)realloc(v.data, v.cap * sizeof(Token));
        }
        v.data[v.size++] = t;
    } while (strcmp(t.nome, "EOF") != 0);

    return v;
}




int main(int argc, char *argv[]) {
    FILE *fp;
    const char *arquivo;

    if (argc >= 2) {
        arquivo = argv[1];
        fp = fopen(arquivo, "r");
    } else {
        arquivo = "codigo.pas";
        fp = fopen(arquivo, "r");
    }

    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo '%s'\n", arquivo);
        return 1;
    }

    FILE *fpLex = fopen("result.lex", "w");
    FILE *fpErr = fopen("result.err", "w");
    FILE *fpTs = fopen("result.ts", "w");

    if (fpLex == NULL || fpErr == NULL || fpTs == NULL) {
        perror("Erro ao abrir arquivos de saida");
        if (fpLex != NULL) fclose(fpLex);
        if (fpErr != NULL) fclose(fpErr);
        if (fpTs != NULL) fclose(fpTs);
        return 1;
    }

    initTS();

    TokenVec v = tokenize_all(fp);

    for (int i = 0; i < v.size; i++) {
        Token t = v.data[i];
        if (strcmp(t.nome, "ERROR") == 0) {
            writeErr(fpErr, t.tipo, t.valor, t.linha, t.coluna);
        } else if (strcmp(t.nome, "EOF") != 0) {
            writeLex(fpLex, t);
        }
    }

    // CHAMADA DO SINTÁTICO:
    parse(&v, fpErr);

    tv_free(&v);

    writeTS(fpTs);

    fclose(fpLex);
    fclose(fpErr);
    fclose(fpTs);
    fclose(fp);

    printf("Analise lexica finalizada.\n");
    printf("Arquivos gerados: result.lex, result.err, result.ts\n");

    return 0;
}