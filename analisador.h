#ifndef ANALISADOR_H
#define ANALISADOR_H

#include <stdio.h>

#define MAX_LEXEMA 100
#define MAX_TS 1000

// Palavras reservadas
extern int qtd_kw_program;
extern int qtd_kw_var;
extern int qtd_kw_begin;
extern int qtd_kw_end;
extern int qtd_kw_if;
extern int qtd_kw_then;
extern int qtd_kw_else;
extern int qtd_kw_while;
extern int qtd_kw_do;
extern int qtd_kw_integer;
extern int qtd_kw_real;

// Identificadores
extern int qtd_id;

// Numeros
extern int qtd_num_int;
extern int qtd_num_real;

// Simbolos
extern int qtd_smb_sem;   // ;
extern int qtd_smb_com;   // ,
extern int qtd_smb_opa;   // (
extern int qtd_smb_cpa;   // )
extern int qtd_smb_col;   // :
extern int qtd_smb_dot;   // .
extern int qtd_smb_obc;   // {
extern int qtd_smb_cbc;   // }

// Operadores
extern int qtd_op_eq;     // =
extern int qtd_op_ne;     // <>
extern int qtd_op_lt;     // <
extern int qtd_op_le;     // <=
extern int qtd_op_gt;     // >
extern int qtd_op_ge;     // >=
extern int qtd_op_ass;    // :=
extern int qtd_op_ad;     // +
extern int qtd_op_min;    // -
extern int qtd_op_mul;    // *
extern int qtd_op_div;    // /

extern int qtd_erro;

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

extern Simbolo tabela_ts[MAX_TS];
extern int qtd_ts;

extern int linha;
extern int coluna;

void toLower(char *dst, const char *src, int max);
int tsExiste(const char *lexema);
void addTS(const char *lexema, const char *tipo);
void initTS(void);
Token createSimpleToken(const char *nome, const char *tipo, const char *valor, int linha, int coluna);
void writeLex(FILE *fpLex, Token token);
void writeErr(FILE *fpErr, const char *erro, const char *valor, int linha, int coluna);
void writeTS(FILE *fpTs);
Token createWordOrNumberToken(const char *lexemaOriginal, int linha, int coluna);
Token proximoToken(FILE *fp);

typedef struct {
    Token *data;
    int size;
    int cap;
} TokenVec;

TokenVec tokenize_all(FILE *fp);
void tv_free(TokenVec *v);

#endif