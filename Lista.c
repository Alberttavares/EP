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
    AVISO(Lista.c : Ainda não implementei a função 'liberaLista'); // Retire esssa mensagem ao implementar a fução

    // Com você :)

    
}

/**
 * @brief Insere um novo elemento no fim da lista
 * 
 * @param lista Ponteiro para a lista
 * @param pos Posição a ser inserida
 */
void insereLista(Lista *lista, Posicao pos) {
    AVISO(Lista.c : Ainda não implementei a função 'insereLista'); // Retire esssa mensagem ao implementar a fução

    // Com você :)

    
}

/**
 * @brief Inclui os elementes da lista 'origem' ao final da lista 'destino'
 *
 * @param destino Lista de destino
 * @param origem Lista de origem
 */
void appendLista(Lista *destino, const Lista *origem){
    AVISO(Lista.c : Ainda não implementei a função 'appendLista'); // Retire esssa mensagem ao implementar a fução

    //Com você :)

    
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




