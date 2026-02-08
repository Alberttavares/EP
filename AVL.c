#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AVL.h"

/**
 * @brief Estrutura do nó da árvore AVL
 * 
 */
typedef struct noBST {
    Cor pixel;
    Lista *ocorrencias;
    int altura;
    struct noBST *esq;
    struct noBST *dir;
} NoBST;

/**
 * @brief Estrutura da árvore AVL
 * 
 */
struct arvore {
    NoBST *raiz;
    int numeroNos;
};

/**
 * @brief Função auxiliar para criar um novo nó da árvore
 * 
 * @param cor Cor do pixel
 * @param p Posição do pixel na imagem
 * @return NoBST* Ponteiro para o novo nó criado
 */
NoBST *criaNo(Cor cor, Posicao p) {
    NoBST *no = mallocSafe(sizeof(NoBST));
    no->pixel = cor;
    no->ocorrencias = criaLista();
    no->altura = 0;
    insereLista(no->ocorrencias, p);
    no->esq = NULL;
    no->dir = NULL;
    return no;
}

/**
 * @brief Cria uma nova árvore de busca binária a partir de uma imagem
 * 
 * @param img Imagem fonte para construção da árvore
 * @return Arvore* Ponteiro para a árvore criada
 */
Arvore *criaArvore(Imagem *img) {
    if(img == NULL) {
        ERRO("Imagem inválida para criação da árvore.");
        return NULL;
    }
    AVISO(AVL.c : Ainda não implementei a função 'criaArvore'); // Retire esssa mensagem ao implementar a fução
    Arvore *arvore = mallocSafe(sizeof(Arvore));
    
    return arvore;
}



/**
 * @brief Libera os nós da árvore recursivamente
 * 
 * @param raiz Ponteiro para o nó raiz
 */
void liberaNosArvore(NoBST *raiz){
    AVISO(AVL.c : Ainda não implementei a função 'liberaNosArvore'); // Retire esssa mensagem ao implementar a fução
    
}

/**
 * @brief Libera a árvore inteira
 * 
 * @param arvore Ponteiro para a árvore
 */
void liberaArvore(Arvore *arvore) {
    if(arvore == NULL)
        return;
    liberaNosArvore(arvore->raiz);
    free(arvore);
}

/**
 * @brief Retorna a altura de um nó
 * 
 * @param raiz Ponteiro para o nó
 * @return int Altura do nó
 */
int altura(NoBST *raiz) {
    if(raiz == NULL) return -1;
    return raiz->altura;
}

/**
 * @brief Calcula o fator de balanceamento de um nó
 * 
 * @param raiz Ponteiro para o nó
 * @return int Fator de balanceamento
 */
int fatorBalanceamento(NoBST *raiz) {
    if (raiz == NULL) return 0;
    return altura(raiz->esq) - altura(raiz->dir);
}

/**
 * @brief Realiza uma rotação à direita em um nó
 * 
 * @param A Nó a ser rotacionado
 * @return NoBST* Novo nó raiz após a rotação
 */
NoBST *rotacaoDireita(NoBST *A) {
    NoBST *B = A->esq;

    //Realiza a rotação
    A->esq = B->dir;
    B->dir = A;

    //Atualiza a altura dos nós
    A->altura = max(altura(A->dir), altura(A->esq)) + 1;
    B->altura = max(altura(B->dir), altura(B->esq)) + 1;
    
    //Retorna a nova raiz
    return B;
}

/**
 * @brief Realiza uma rotação à esquerda em um nó
 * 
 * @param A Nó a ser rotacionado
 * @return NoBST* Novo nó raiz após a rotação
 */
NoBST *rotacaoEsquerda(NoBST *A) {
    NoBST *B = A->dir;

    //Realiza a rotação
    A->dir = B->esq;
    B->esq = A;

    //Atualiza a altura dos nós
    A->altura = max(altura(A->dir), altura(A->esq)) + 1;
    B->altura = max(altura(B->dir), altura(B->esq)) + 1;

    //Retorna a nova raiz
    return B;
}

/**
 * @brief Insere um novo nó na árvore
 * 
 * @param raiz Ponteiro para o nó raiz
 * @param cor Cor do pixel
 * @param p Posição do pixel na imagem
 * @param novoNoInserido Novo nó foi inserido (true/false)
 * @return NoBST* Novo nó raiz após a inserção
 */
NoBST *insereNo(NoBST *raiz, Cor cor, Posicao p, bool *novoNoInserido) {
    // PASSO 1: Fazer a inserção normal de uma BST
    if(raiz == NULL) {
        *novoNoInserido = true;
        return criaNo(cor, p);
    }

    // Lembre-se que se a cor já existir na árvore, você deve apenas
    // adicionar a nova posição à lista de ocorrências do nó existente.
    // Use a função comparaCores (Cor.c) para comparar as cores.
    // Além disso, após a inserção, você deve atualizar as alturas e
    // verificar se é necessário fazer rotações para manter a árvore balanceada.

    AVISO(AVL.c : Ainda não implementei a função 'insereNo'); // Retire esssa mensagem ao implementar a fução

    // Com você :)

    
    return raiz;
}

/**
 * @brief Insere uma cor e sua posição na árvore
 * 
 * @param arvore Ponteiro para a árvore
 * @param cor Cor do pixel
 * @param p Posição do pixel na imagem
 */
void insereArvore(Arvore *arvore, Cor cor, Posicao p) {
    bool novoNoInserido = false;
    arvore->raiz = insereNo(arvore->raiz, cor, p, &novoNoInserido);
    if (novoNoInserido){
        arvore->numeroNos++;
    }
}

/**
 * @brief Busca por uma cor exata na árvore
 * 
 * @param raiz Ponteiro para o nó raiz
 * @param cor Cor a ser buscada
 * @return Lista* Lista de posições onde a cor foi encontrada ou NULL se não encontrada
 */
Lista* buscaCorExata (NoBST* raiz, Cor cor) {
    // Nesta função você deve percorrer a árvore comparando a cor buscada
    // com a cor do nó atual. Se encontrar a cor, deve retornar o ponteiro
    // da lista de ocorrências do nó. Se não encontrar, deve retornar NULL.

    // Use a função comparaCores (Cor.c) para comparar as cores.

    AVISO(AVL.c : Ainda não implementei a função 'buscaCorExata'); // Retire esssa mensagem ao implementar a fução
    // Com você :)
    return NULL;
}

/**
 * @brief Busca por uma cor aproximada na árvore dentro de uma tolerância
 * 
 * @param raiz Ponteiro para o nó raiz
 * @param cor Cor a ser buscada
 * @param tolerancia Tolerância para a busca
 * @param resultado Lista de posições onde a cor foi encontrada
 */
void buscaCorAproximada(NoBST* raiz, Cor cor, int tolerancia, Lista **resultado) {
    // Nesta função você deve percorrer a árvore inteira, verificando se a cor do nó
    // está dentro da tolerância, ou seja, se distanciaCores(raiz->pixel, cor) <= tolerancia.
    // Se estiver, você deve adicionar todas as posições
    // da lista de ocorrências do nó à lista de resultado (use a função appendLista).

    AVISO(AVL.c : Ainda não implementei a função 'buscaCorAproximada'); // Retire esssa mensagem ao implementar a fução
    // Com você :)
    
}

/**
 * @brief Busca por uma cor aproximada na árvore dentro de uma tolerância.
 * Se a torlerancia for 0, retorna o ponteiro da lista do nó correspondente. 
 * Caso contrário, retorna uma NOVA lista com todas as ocorrências dentro da tolerância.
 * 
 * @param arvore Ponteiro para a árvore
 * @param cor Cor a ser buscada
 * @param tolerancia Tolerância para a busca
 * @return Lista* Lista de posições onde a cor foi encontrada ou NULL se não encontrada
 */
Lista *buscaArvore(Arvore *arvore, Cor cor, int tolerancia) {
    // Nesta função você deve verificar se a árvore é válida.
    // Se for, deve chamar a função de busca apropriada dependendo
    // do valor da tolerância.
    if (arvoreVazia(arvore)) {
        ERRO("Árvore inválida para busca.");
        return NULL;
    }

    AVISO(AVL.c : Ainda não implementei a função 'buscaArvore'); // Retire esssa mensagem ao implementar a fução

    // Com você :)

    return NULL;
}

/**
 * @brief Retorna a altura da árvore
 * 
 * @param arvore Ponteiro para a árvore
 * @return int Altura da árvore
 */
int alturaArvore(Arvore *arvore){
    return altura(arvore->raiz);
}

/**
 * @brief Retorna o número de nós na árvore
 * 
 * @param arvore Ponteiro para a árvore
 * @return int Número de nós na árvore
 */
int numeroNosArvore(Arvore *arvore) {
    return arvore->numeroNos;
}

/**
 * @brief Verifica se a árvore está vazia
 * 
 * @param arvore Ponteiro para a árvore
 * @return true Se a árvore estiver vazia
 * @return false Caso contrário
 */
bool arvoreVazia(Arvore *arvore) {
    return arvore == NULL || arvore->raiz == NULL;
}