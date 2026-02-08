#include <stdlib.h> 
#include <stdio.h>  
#include <math.h>

#include "Cor.h"

/**
 * @brief Ajusta o valor de uma cor para o intervalo [0, 255]
 * 
 * @param v Valor da cor a ser ajustado
 * @return Byte Valor ajustado
 */
Byte ajustaCor(int v) {
    return (v < 0) ? 0 : (v > 255) ? 255 : v;
}


/**
 * @brief Compara duas cores (critério lexicográfico R->G->B) e retorna:
 * - 0 se as cores são iguais
 * - um valor negativo se a primeira cor é menor que a segunda
 * - um valor positivo se a primeira cor é maior que a segunda
 *
 * @param a Primeira cor
 * @param b Segunda cor
 * @return int Retorna 0 se as cores são iguais, um valor negativo se a primeira é menor que a segunda, e um valor positivo se a primeira é maior que a segunda
 */
int comparaCores(Cor a, Cor b) {
    if (a.r != b.r) return a.r - b.r;
    if (a.g != b.g) return a.g - b.g;
    return a.b - b.b;
}


/**
 * @brief Verifica se duas cores são iguais
 * 
 * @param a Primeira cor
 * @param b Segunda cor
 * @return bool Retorna true se as cores são iguais, false caso contrário
 */
bool coresIguais(Cor a, Cor b) {
    return (a.r == b.r && a.g == b.g && a.b == b.b);
}

/**
 * @brief Calcula a distância entre duas cores (para busca aproximada)
 *
 * @param a Primeira cor
 * @param b Segunda cor
 * @return double Distância entre as cores
 */
double distanciaCores(Cor a, Cor b) {
    int dr = a.r - b.r;
    int dg = a.g - b.g;
    int db = a.b - b.b;
    return sqrt(dr * dr + dg * dg + db * db);
}

/**
 * @brief Imprime uma cor em formato legível
 * 
 * @param c Cor a ser impressa
 */
void imprimeCor(Cor c) {
    printf("RGB(%d, %d, %d)", c.r, c.g, c.b);
}
