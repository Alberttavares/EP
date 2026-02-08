#include <stdio.h>
#include <stdlib.h>

#include "Pilha.h"

/**
 * @brief Estrutura de dados para representar um nó da pilha
 * 
 */
typedef struct no {
    Imagem *imagem;
    struct no *proximo;
} No;

/**
 * @brief Estrutura de dados para representar uma pilha
 * 
 */
struct pilha {
    No *inicio;
    int n;
};

/**
 * @brief Cria uma nova pilha vazia
 * 
 * @return Pilha* Ponteiro para a pilha criada
 */
Pilha *criaPilha() {
    Pilha *p = malloc(sizeof(Pilha));
    p->inicio = NULL;
    p->n = 0;
    return p;
}

/**
 * @brief Libera a memória alocada para uma pilha
 * 
 * @param pilha Ponteiro para a pilha a ser liberada
 */
void liberaPilha(Pilha *pilha) {
    AVISO(Pilha.c : Ainda não implementei a função 'liberaPilha');

    // Com você :)


}

/**
 * @brief Adiciona um elemento na pilha
 * 
 * @param pilha Ponteiro para a pilha
 * @param img Ponteiro para a imagem a ser adicionada
 */
void pushPilha(Pilha *pilha, Imagem *img) {
    /* ATENÇÃO: passe sempre uma CÓPIA da imagem */
    AVISO(Pilha.c : Ainda não implementei a função 'pushPilha');

    // Com você :)

    
}

/**
 * @brief Remove o elemento do topo da pilha
 * 
 * @param pilha Ponteiro para a pilha
 */
void popPilha(Pilha *pilha) {
    /* ATENÇÃO: não se esqueça de liberar a imagem alocada */
    AVISO(Pilha.c : Ainda não implementei a função 'popPilha');

    // Com você :)

    
}

/**
 * @brief Retorna o elemento do topo da pilha
 * 
 * @param pilha Ponteiro para a pilha
 * @return Imagem* Ponteiro para a imagem do topo da pilha
 */
Imagem *topPilha(Pilha *pilha){
    AVISO(Pilha.c : Ainda não implementei a função 'topPilha'); 
    
    // Com você :)

    return NULL;
    
}

/**
 * @brief Verifica se a pilha está vazia
 * 
 * @param pilha Ponteiro para a pilha
 * @return true Se a pilha estiver vazia
 * @return false Se a pilha não estiver vazia
 */
bool pilhaVazia(Pilha *pilha) {
    return pilha == NULL || pilha->inicio == NULL;
}

/**
 * @brief Retorna o tamanho da pilha
 * 
 * @param pilha Ponteiro para a pilha
 * @return int Tamanho da pilha
 */
int tamanhoPilha(Pilha *pilha) {
    return pilha->n;
}
