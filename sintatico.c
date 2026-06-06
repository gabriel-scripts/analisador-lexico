#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sintatico.h"

// ---------------------------------------------------------
// FUNÇÕES DE INFRAESTRUTURA (Professor Style)
// ---------------------------------------------------------

// Imprime erro e encerra
static void perr(Parser *p, const char *msg) {
    Token t = p->v->data[p->i < p->v->size ? p->i : p->v->size - 1];
    fprintf(stderr, "ERRO SINTATICO (Linha %d, Coluna %d): %s. Encontrado: '%s'\n", 
            t.linha, t.coluna, msg, t.valor);
    exit(1);
}

// Retorna o token atual
static Token cur(Parser *p) {
    if (p->i < p->v->size) return p->v->data[p->i];
    return p->v->data[p->v->size - 1]; // Geralmente o EOF
}

// Verifica se o token atual é o esperado. Se for, avança e retorna 1.
static int match(Parser *p, const char *nomeEsperado) {
    if (strcmp(cur(p).nome, nomeEsperado) == 0) {
        p->i++;
        return 1;
    }
    return 0;
}

// Exige que o token atual seja o esperado. Se não for, dispara erro fatal.
static void expect(Parser *p, const char *nomeEsperado, const char *msgErro) {
    if (!match(p, nomeEsperado)) {
        perr(p, msgErro);
    }
}

// ---------------------------------------------------------
// FUNÇÕES DA GRAMÁTICA (Para você implementar)
// ---------------------------------------------------------

static void parse_block(Parser *p) {
    // Exemplo:
    // se match(p, "KW_VAR"):
    //   parse_variable_declaration()
    //
    // expect(p, "KW_BEGIN", "Esperava 'begin'");
    // parse_statements()
    // expect(p, "KW_END", "Esperava 'end'");
}

static void parse_program(Parser *p) {
    // Todo programa comeca com 'program'
    expect(p, "KW_PROGRAM", "Esperava a palavra reservada 'program'");
    
    // Nome do programa
    expect(p, "ID", "Esperava o identificador do programa");
    
    // Ponto e virgula
    expect(p, "SMB_SEM", "Esperava ';' apos o nome do programa");
    
    // Bloco principal (A DEFINIR)
    // parse_block(p); 
    
    // Ponto final
    expect(p, "SMB_DOT", "Esperava '.' no final do arquivo");
}

// ---------------------------------------------------------
// PONTO DE ENTRADA DO SINTÁTICO
// ---------------------------------------------------------
void parse(const TokenVec *v) {
    Parser p = { v, 0 };
    
    parse_program(&p);
    
    // Se o parse_program rodou e sobrou token (que nao seja EOF), tem algo errado
    if (!match(&p, "EOF")) {
        perr(&p, "Simbolos extra encontrados apos o fim do programa");
    }
    
    printf("Analise sintatica concluida com sucesso!\n");
}