#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEXEMA 100
#define MAX_TS 1000

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

typedef struct Token {
    char nome[20];
    char tipo[20];
    char valor[50];
    int linha;
    int coluna;
} Token;

typedef struct Simbolo {
    char lexema[50];
    char tipo[20];
} Simbolo;

Simbolo tabela_ts[MAX_TS];
int qtd_ts = 0;

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

void lexicalAnalisis(FILE *fp) {
    int c;
    int linha = 1;
    int coluna = 1;
    char lexema[MAX_LEXEMA];
    FILE *fpLex = fopen("result.lex", "w");
    FILE *fpErr = fopen("result.err", "w");
    FILE *fpTs = fopen("result.ts", "w");

    if (fpLex == NULL || fpErr == NULL || fpTs == NULL) {
        perror("Erro ao abrir arquivos de saida");
        if (fpLex != NULL) fclose(fpLex);
        if (fpErr != NULL) fclose(fpErr);
        if (fpTs != NULL) fclose(fpTs);
        return;
    }

    initTS();

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
                writeErr(fpErr, "ERRO_COMENTARIO_NAO_FECHADO", "{", tokenLinha, tokenColuna);
                break;
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
                    writeErr(fpErr, "ERRO_STRING_NAO_FECHADA", "'", tokenLinha, tokenColuna);
                    linha++;
                    coluna = 1;
                    break;
                }
                coluna++;
            }
            if (c == EOF && !fechado) {
                qtd_erro++;
                writeErr(fpErr, "ERRO_STRING_NAO_FECHADA", "'", tokenLinha, tokenColuna);
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
            writeLex(fpLex, createWordOrNumberToken(lexema, tokenLinha, tokenColuna));
            continue;
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
            writeLex(fpLex, createWordOrNumberToken(lexema, tokenLinha, tokenColuna));
            continue;
        }

        if (c == ';') {
            qtd_smb_sem++;
            writeLex(fpLex, createSimpleToken("SMB_SEM", "simbolo", ";", tokenLinha, tokenColuna));
            coluna++;
            continue;
        }
        if (c == ',') {
            qtd_smb_com++;
            writeLex(fpLex, createSimpleToken("SMB_COM", "simbolo", ",", tokenLinha, tokenColuna));
            coluna++;
            continue;
        }
        if (c == '(') {
            qtd_smb_opa++;
            writeLex(fpLex, createSimpleToken("SMB_OPA", "simbolo", "(", tokenLinha, tokenColuna));
            coluna++;
            continue;
        }
        if (c == ')') {
            qtd_smb_cpa++;
            writeLex(fpLex, createSimpleToken("SMB_CPA", "simbolo", ")", tokenLinha, tokenColuna));
            coluna++;
            continue;
        }
        if (c == '.') {
            qtd_smb_dot++;
            writeLex(fpLex, createSimpleToken("SMB_DOT", "simbolo", ".", tokenLinha, tokenColuna));
            coluna++;
            continue;
        }
        if (c == '+') {
            qtd_op_ad++;
            writeLex(fpLex, createSimpleToken("OP_AD", "operador", "+", tokenLinha, tokenColuna));
            coluna++;
            continue;
        }
        if (c == '-') {
            qtd_op_min++;
            writeLex(fpLex, createSimpleToken("OP_MIN", "operador", "-", tokenLinha, tokenColuna));
            coluna++;
            continue;
        }
        if (c == '*') {
            qtd_op_mul++;
            writeLex(fpLex, createSimpleToken("OP_MUL", "operador", "*", tokenLinha, tokenColuna));
            coluna++;
            continue;
        }
        if (c == '/') {
            qtd_op_div++;
            writeLex(fpLex, createSimpleToken("OP_DIV", "operador", "/", tokenLinha, tokenColuna));
            coluna++;
            continue;
        }
        if (c == '=') {
            qtd_op_eq++;
            writeLex(fpLex, createSimpleToken("OP_EQ", "operador", "=", tokenLinha, tokenColuna));
            coluna++;
            continue;
        }

        if (c == ':') {
            int prox = fgetc(fp);
            if (prox == '=') {
                qtd_op_ass++;
                writeLex(fpLex, createSimpleToken("OP_ASS", "operador", ":=", tokenLinha, tokenColuna));
                coluna += 2;
            } else {
                if (prox != EOF) {
                    ungetc(prox, fp);
                }
                qtd_smb_col++;
                writeLex(fpLex, createSimpleToken("SMB_COL", "simbolo", ":", tokenLinha, tokenColuna));
                coluna++;
            }
            continue;
        }

        if (c == '<') {
            int prox = fgetc(fp);
            if (prox == '=') {
                qtd_op_le++;
                writeLex(fpLex, createSimpleToken("OP_LE", "operador", "<=", tokenLinha, tokenColuna));
                coluna += 2;
            } else if (prox == '>') {
                qtd_op_ne++;
                writeLex(fpLex, createSimpleToken("OP_NE", "operador", "<>", tokenLinha, tokenColuna));
                coluna += 2;
            } else {
                if (prox != EOF) {
                    ungetc(prox, fp);
                }
                qtd_op_lt++;
                writeLex(fpLex, createSimpleToken("OP_LT", "operador", "<", tokenLinha, tokenColuna));
                coluna++;
            }
            continue;
        }

        if (c == '>') {
            int prox = fgetc(fp);
            if (prox == '=') {
                qtd_op_ge++;
                writeLex(fpLex, createSimpleToken("OP_GE", "operador", ">=", tokenLinha, tokenColuna));
                coluna += 2;
            } else {
                if (prox != EOF) {
                    ungetc(prox, fp);
                }
                qtd_op_gt++;
                writeLex(fpLex, createSimpleToken("OP_GT", "operador", ">", tokenLinha, tokenColuna));
                coluna++;
            }
            continue;
        }

        qtd_erro++;
        {
            char invalido[2];
            invalido[0] = (char)c;
            invalido[1] = '\0';
            writeErr(fpErr, "ERRO_CARACTERE_INVALIDO", invalido, tokenLinha, tokenColuna);
        }
        coluna++;
    }

    writeTS(fpTs);

    fclose(fpLex);
    fclose(fpErr);
    fclose(fpTs);
}

int main(void) {
    FILE *fp;

    fp = fopen("micropascal/micropascal.pas", "r");
    if (fp == NULL) {
        fp = fopen("codigo.pas", "r");
    }
    if (fp == NULL) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    lexicalAnalisis(fp);

    fclose(fp);

    printf("Analise lexica finalizada.\n");
    printf("Arquivos gerados: result.lex, result.err, result.ts\n");

    return 0;
}