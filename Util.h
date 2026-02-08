#ifndef __UTIL_H
#define __UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Etapa atual do EP
 * 
 */
#define ETAPA 0

#define ERRO(msg) fprintf(stderr, "\033[31mERRO:\033[0m %s\n", #msg)
#define AVISO(msg) fprintf(stdout, "AVISO: %s\n", #msg)

/**
 * @brief Macro para testar uma condição e exibir uma mensagem de erro ou sucesso.
 * 
 */
#define TESTE(condicao, mensagem)                           \
    if (!(condicao)) {                                      \
        printf("❌ \033[31mFALHOU:\033[0m %s\n", mensagem); \
        printf("   Linha: %d\n", __LINE__);                 \
        exit(EXIT_FAILURE);                                 \
    }                                                       \
    else {                                                  \
        printf("✅ \033[32mPASSOU:\033[0m %s\n", mensagem); \
    }    

/**
 * @brief Tipo para representar um valor de cor em uma imagem.
 *
 */
/* Byte = valores entre 0 e 255 (8 bits)*/
typedef unsigned char Byte;

/**
 * @brief Estrutura para representar uma posição em uma imagem
 * 
 */
typedef struct {
    int linha, coluna;
} Posicao;

void *mallocSafe(size_t nbytes);

int max(int a, int b);

#endif