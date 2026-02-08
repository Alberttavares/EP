#ifndef __COR_H
#define __COR_H

#include <stdio.h>
#include <stdlib.h>
#include "Util.h"

typedef struct {
    Byte r, g, b;
} Cor;

Byte ajustaCor(int v);

int comparaCores(Cor a, Cor b);

bool coresIguais(Cor a, Cor b);

double distanciaCores(Cor a, Cor b);

void imprimeCor(Cor c);

#endif