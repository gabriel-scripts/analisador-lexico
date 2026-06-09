#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sintatico.h"

// As mensagens devem ter os seguintes formatos (onde nn é o número da linha onde o erro foi detectado e
// lex é o lexema encontrado):
// • nn:token nao esperado [lex].
// • nn:fim de arquivo não esperado.

// Imprime erro e encerra
static void perr(Parser *p, const char *msg) {
    Token t = p->v->data[p->i < p->v->size ? p->i : p->v->size - 1];
    fprintf(stderr, "%d:%d: [%s] \n", 
            t.linha, t.coluna, t.tipo);
    exit(1);
}

// Retorna o token atual
static Token cur(Parser *p) {
    if (p->i < p->v->size) return p->v->data[p->i];
    return p->v->data[p->v->size - 1]; 
}

// Verifica se o token atual é o esperado, mas não avança. Retorna 1 se for, senão 0.
static int peek(Parser *p, const char *nomeEsperado) {
    if (strcmp(cur(p).nome, nomeEsperado) == 0) {
        return 1;
    }
    return 0;
}

// Verifica se o token atual é o esperado. Se for, avança e retorna 1. Senão, retorna 0.
static int match(Parser *p, const char *nomeEsperado) {
    if (strcmp(cur(p).nome, nomeEsperado) == 0) {
        p->i++;
        return 1;
    }
    return 0;
}

// Exige que o token atual seja o esperado. Se não for, dispara erro!
static void expect(Parser *p, const char *nomeEsperado, const char *msgErro) {
    if (!match(p, nomeEsperado)) {
        perr(p, msgErro);
    }
}
// COMANDOS
// <comando composto> ::= begin <comando> ; { <comando> ; } end
// <comando> ::= <atribuição> | <comando composto> | <comando condicional> | <comando repetitivo>
// <atribuição> ::= <variável> := <expressão>
// <comando condicional> ::= if <expressão> then <comando> [ else <comando> ]
// <comando repetitivo> ::= while <expressão> do <comando>

//
// <expressão> ::= <expressão simples> [ <relação> <expressão simples> ]
// <relação> ::= = | <> | < | <= | >= | >
// <expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
// <termo> ::= <fator> { ( * | / ) <fator> }
// <fator> ::= <variável> | <número> | ( <expressão> )
// <variável> ::= <identificador>


static void parse_factor(Parser *p){
    if(peek(p, "ID")) {
        expect(p, "ID", "Esperava um identificador");
    } else if (peek(p, "NUM_INT") || peek(p, "NUM_REAL")) {
        p->i++;
    } else if (peek(p, "SMB_OP_PAR")) {
        expect(p, "SMB_OP_PAR", "Esperava '(' para iniciar uma expressão entre parênteses");
        parse_expression(p);
        expect(p, "SMB_CL_PAR", "Esperava ')' para fechar a expressão entre parênteses");
    } else {
        perr(p, "Esperava um fator (identificador, número ou expressão entre parênteses)");
    }
}

static void parse_term(Parser *p){
    parse_factor(p);
    while (peek(p, "OP_MULT") || peek(p, "OP_DIV")) {
        p->i++;
        parse_factor(p);
    }
}

static void parse_relation(Parser *p) {
    if (peek(p, "OP_EQ") || peek(p, "OP_NEQ") || peek(p, "OP_LT") || peek(p, "OP_LTE") || peek(p, "OP_GT") || peek(p, "OP_GTE")) {
        p->i++;
    } else {
        perr(p, "Esperava um operador relacional (=, <>, <, <=, > ou >=)");
    }
}

static void parse_simple_expression(Parser *p) {
    do{
        if (peek(p, "OP_SUM") || peek(p, "OP_SUB")) {
            p->i++;
        }
        parse_term(p);
    } while (peek(p, "OP_SUM") || peek(p, "OP_SUB"));
}

static void parse_expression(Parser *p) {
    parse_simple_expression(p);
    if (relation_helper(p)) {
        parse_relation(p);
        parse_simple_expression(p);
    }
}

static void parse_repetitive_command(Parser *p) {
    expect(p, "KW_WHILE", "Esperava 'while' para iniciar um comando repetitivo");
    parse_expression(p);
    expect(p, "KW_DO", "Esperava 'do' após a expressão de um comando repetitivo");
    parse_command(p);
}

static void parse_conditional_command(Parser *p) {
    expect(p, "KW_IF", "Esperava 'if' para iniciar um comando condicional");
    parse_expression(p);
    expect(p, "KW_THEN", "Esperava 'then' após a expressão de um comando condicional");
    parse_command(p);
    if (match(p, "KW_ELSE")) {
        parse_command(p);
    }
}

static void parse_atribution(Parser *p) {
    expect(p, "ID", "Esperava um identificador para iniciar uma atribuição");
    expect(p, "OP_ASS", "Esperava ':=' após o identificador");
    parse_expression(p);
}

static void parse_command(Parser *p) {
    if(peek(p, "KW_IF")) {
        parse_conditional_command(p);
    } else if (peek(p, "KW_WHILE")) {
        parse_repetitive_command(p);
    } else if (peek(p, "KW_BEGIN")) {
        parse_compound_command(p);
    } else {
        parse_atribution(p);
    }
}

static void parse_compound_command(Parser *p) {
    expect(p, "KW_BEGIN", "Esperava 'begin' para iniciar um comando composto");
    do
    {
        parse_command(p);
        expect(p, "SMB_SEM", "Esperava ';' para separar comandos dentro de um comando composto");
    } while (!match(p, "SMB_COM"));
    
    parse_compound_command(p);
    expect(p, "KW_END", "Esperava 'end' para finalizar um comando composto");
}


static void parse_type(Parser *p) {
    if (match(p, "KW_INTEGER")) {
        return;
    } else if (match(p, "KW_REAL")) {
        return;
    } else {
        perr(p, "Esperava 'integer' ou 'real' para tipo de variável");
    }
}

static void parse_identifiers_list(Parser *p) {
    expect(p, "ID", "Esperava um identificador");
    while (match(p, "SMB_COM")) {
        expect(p, "ID", "Esperava um identificador após ','");
    }
}


static void parse_variable_declaration(Parser *p) {
    parse_identifiers_list(p);
    expect(p, "OP_ASS", "Esperava ':=' após a lista de identificadores");
    parse_type(p);
}

static void parse_variable_declaration_block(Parser *p) {
    if(match(p, "KW_VAR")){
        do{
            parse_variable_declaration(p);
            expect(p, "SMB_SEM", "Esperava ';' no final da declaração de variáveis");
        }while(peek(p, "ID")); // Verifica se o próximo token é um identificador, se for, tem mais declaração de variável. Se não for, acabou o bloco de declaração de variáveis
    }
}

static void parse_block(Parser *p) {

    parse_variable_declaration_block(p);
    parse_compound_command(p);

}

static void parse_program(Parser *p) {
    expect(p, "KW_PROGRAM", "Esperava a palavra reservada 'program'");
    expect(p, "ID", "Esperava o identificador do programa");
    expect(p, "SMB_SEM", "Esperava ';' apos o nome do programa");
    parse_block(p); 
    expect(p, "SMB_DOT", "Esperava '.' no final do arquivo");
}


void parse(const TokenVec *v) {
    Parser p = { v, 0 };
    
    parse_program(&p);
    
    // Se o parse_program rodou e sobrou token (que nao seja EOF), tem algo errado
    if (!match(&p, "EOF")) {
        perr(&p, "Simbolos extra encontrados apos o fim do programa");
    }
    
    printf("Analise sintatica concluida com sucesso!\n");
}


static Parser* relation_helper(Parser *p) {
    if (peek(p, "OP_EQ") || peek(p, "OP_NEQ") || peek(p, "OP_LT") || peek(p, "OP_LTE") || peek(p, "OP_GT") || peek(p, "OP_GTE")) {
        return 1;
    } else {
        return 0;
    }
}