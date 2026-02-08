#ifndef __LISTA_H_
#define __LISTA_H_

#include "Util.h"


typedef struct no {
    Posicao pos;
    struct no *proximo;
} No;

typedef struct lista {
    No *inicio;
    No *fim;
    int tamanho;
} Lista;

Lista *criaLista();

void liberaLista(Lista* lista);

void insereLista(Lista *lista, Posicao p);

void appendLista(Lista *destino, const Lista *origem);

void imprimeLista(const Lista *lista);

bool listaVazia(const Lista *lista);

int tamanhoLista(const Lista *lista);


#endif
