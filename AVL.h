#ifndef __ARVORE_H
#define __ARVORE_H

#include "Cor.h"
#include "Lista.h"
#include "Imagem.h"
#include "Util.h"

typedef struct arvore Arvore;

Arvore *criaArvore(Imagem *img);

void liberaArvore(Arvore *);

bool arvoreVazia(Arvore *);

Lista *buscaArvore(Arvore *, Cor, int);

void insereArvore(Arvore *, Cor, Posicao);

int numeroNosArvore(Arvore *);

int alturaArvore(Arvore *);

#endif