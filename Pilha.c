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
    // Detalhes da Função:
    // Libera a imagem armazenada no nó.

    if (pilha == NULL) return;

    No *atual = pilha->inicio;
    while (atual != NULL) {
        No *aux = atual;
        atual = atual->proximo;
        
        if (aux->imagem != NULL) {
            liberaImagem(aux->imagem);
        }
        free(aux);
    }
    
    free(pilha);
}

/**
 * @brief Adiciona um elemento na pilha
 * 
 * @param pilha Ponteiro para a pilha
 * @param img Ponteiro para a imagem a ser adicionada
 */
void pushPilha(Pilha *pilha, Imagem *img) {
    // Detalhes da Função:
    // Configura o novo nó.
    // Atualiza o topo da pilha.

    if (pilha == NULL || img == NULL) return;

    No *novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) {
        fprintf(stderr, "Erro ao alocar nó da pilha\n");
        return;
    }

    novoNo->imagem = img;
    novoNo->proximo = pilha->inicio;

    pilha->inicio = novoNo;
    pilha->n++;
    
}

/**
 * @brief Remove o elemento do topo da pilha
 * 
 * @param pilha Ponteiro para a pilha
 */
void popPilha(Pilha *pilha) {
    // Detalhes da Função:
    // Atualiza o início para o próximo nó.
    // Libera a imagem associada.

    if (pilhaVazia(pilha)) return;

    No *aux = pilha->inicio;
    
    pilha->inicio = aux->proximo;

    if (aux->imagem != NULL) {
        liberaImagem(aux->imagem);
    }

    free(aux);
    pilha->n--;
    
}

/**
 * @brief Retorna o elemento do topo da pilha
 * 
 * @param pilha Ponteiro para a pilha
 * @return Imagem* Ponteiro para a imagem do topo da pilha
 */
Imagem *topPilha(Pilha *pilha){
    // Detalhes da Função:
    // Retorna o ponteiro para a imagem que esta no topo da pilha.

    if (pilha == NULL || pilha->inicio == NULL) {
        return NULL;
    }
    return pilha->inicio->imagem;
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
