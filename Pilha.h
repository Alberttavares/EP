#ifndef __PILHA_H
#define __PILHA_H

#include "Util.h"
#include "Imagem.h"

typedef struct pilha Pilha;

Pilha *criaPilha();

void liberaPilha(Pilha *);

void pushPilha(Pilha *, Imagem *);

void popPilha(Pilha *);

Imagem *topPilha(Pilha *);

bool pilhaVazia(Pilha *);

bool pilhaCheia(Pilha *);

int tamanhoPilha(Pilha *);

#endif /* __PILHA_H */