#include <stdio.h>
#include <stdlib.h>

#include "Lista.h"

/**
 * @brief Cria uma nova lista vazia
 * 
 * @return Lista* Ponteiro para a lista criada
 */
Lista *criaLista() {
    Lista *lista = malloc(sizeof(Lista));
    lista->inicio = NULL;
    lista->fim = NULL; 
    lista->tamanho = 0;     
    return lista;
}

/**
 * @brief Libera a memória alocada para uma lista
 * 
 * @param lista Ponteiro para a lista a ser liberada
 */
void liberaLista(Lista* lista) {
    if (lista == NULL) return;

    No *atual = lista->inicio;
    while (atual != NULL) {
        No *prox = atual->proximo;
        free(atual);
        atual = prox;
    }

    free(lista);
}

/**
 * @brief Insere um novo elemento no fim da lista
 * 
 * @param lista Ponteiro para a lista
 * @param pos Posição a ser inserida
 */
void insereLista(Lista *lista, Posicao pos) {
    // Detalhes da Função:
    // Aloca o nó usando mallocSafe.
    // Lógica de inserção no fim.
    // Primeiro caso: lista estava vazia.
    // Segundo caso: lista já tinha elementos.
    // último caso: fim nulo mas inicio nao.

    if (lista == NULL) {
        ERRO(insereLista: Lista invalida (NULL));
        return;
    }

    No *novoNo = (No *) mallocSafe(sizeof(No));
    
    novoNo->pos = pos;
    novoNo->proximo = NULL;

    if (lista->inicio == NULL) {
        lista->inicio = novoNo;
        lista->fim = novoNo;
    } else {
        if (lista->fim != NULL) {
            lista->fim->proximo = novoNo;
            lista->fim = novoNo;
        } else {
            No *aux = lista->inicio;
            while(aux->proximo != NULL) aux = aux->proximo;
            aux->proximo = novoNo;
            lista->fim = novoNo;
        }
    }

    lista->tamanho++;
}

/**
 * @brief Inclui os elementes da lista 'origem' ao final da lista 'destino'
 *
 * @param destino Lista de destino
 * @param origem Lista de origem
 */
void appendLista(Lista *destino, const Lista *origem){
    // Detalhes da Função:
    // Ela copia os elementos da lista origem para o final da lista destino
    // Reutilizei a insereLista para facilitar.

    if (destino == NULL || origem == NULL || origem->inicio == NULL) {
        return;
    }

    No *atual = origem->inicio;
    while (atual != NULL) {
        insereLista(destino, atual->pos);
        atual = atual->proximo;
    }
}

/**
 * @brief Imprime os elementos da lista
 * 
 * @param lista Ponteiro para a lista a ser impressa
 */
void imprimeLista(const Lista *lista){
    printf("--- INFORMAÇÃO LISTA ---\n");
    printf("Tamanho: %d\n", tamanhoLista(lista));
    printf("Elementos: ");
    No *atual = lista->inicio;
    // Enquanto não atingirmos o 'fim' da lista, avançamos. 
    while(atual != lista->fim) {
    	printf("(%d, %d) -> ", atual->pos.coluna, atual->pos.linha);
        atual = atual->proximo;
    }
    if (atual) //Imprime a informação do último nó, caso ele exista
        printf("(%d, %d) -> ", atual->pos.coluna, atual->pos.linha);
    printf("NULL\n");
    printf("------------------------\n\n");
}

/**
 * @brief Verifica se a lista está vazia
 * 
 * @param lista Ponteiro para a lista
 * @return true Se a lista estiver vazia
 * @return false Se a lista não estiver vazia
 */
bool listaVazia(const Lista *lista){
    return (lista == NULL || lista->inicio == NULL);
}

/**
 * @brief Retorna o número de elementos da lista
 * 
 * @param lista Ponteiro para a lista
 * @return int Número de elementos na lista
 */
int tamanhoLista(const Lista *lista){
    return lista->tamanho;
}




