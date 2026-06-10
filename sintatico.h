#ifndef SINTATICO_H
#define SINTATICO_H

#include <stdio.h>
#include "analisador.h"

typedef struct {
    const TokenVec *v;
    int i;
    FILE *fpErr;
} Parser;

void parse(const TokenVec *v, FILE *fpErr);

#endif