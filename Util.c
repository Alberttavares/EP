#include <stdlib.h> 
#include <stdio.h>  

#include "Util.h"

/* Fonte:  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html */
void *mallocSafe(size_t nbytes) {
    void *ptr = malloc(nbytes);
    if (ptr == NULL) {
        ERRO(Erro na alocação de memória);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}