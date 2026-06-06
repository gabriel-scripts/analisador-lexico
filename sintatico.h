#ifndef SINTATICO_H
#define SINTATICO_H

#include "analisador.h"

typedef struct {
    const TokenVec *v;
    int i;
} Parser;

void parse(const TokenVec *v);

#endif