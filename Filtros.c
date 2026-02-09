#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Imagem.h"
#include "Filtros.h"

/**
 * @brief Converte uma imagem para escala de cinza
 *
 * @param img Ponteiro para a imagem a ser convertida
 */
void escalaDeCinzaImagem(Imagem *img)
{
    // Detalhes da Função:
    // Cálculos feitos com int/double para evitar a questão do overflow.
    // Ajusta o intervalo
    // Após o cálculo, o pixel é recolorido atribuindo o mesmo valor de cinza.
    // A imagem é modificada diretamente.

    if (!img)
        return;

    int largura = obtemLargura(img);
    int altura = obtemAltura(img);

    for (int y = 0; y < altura; y++)
    {
        for (int x = 0; x < largura; x++)
        {
            Cor c = obtemCorPixel(img, y, x);

            int cinza = (int)(0.30 * c.r + 0.59 * c.g + 0.11 * c.b);

            Byte valorFinal = ajustaCor(cinza);

            Cor novaCor = {valorFinal, valorFinal, valorFinal};
            recolorePixel(img, y, x, novaCor);
        }
    }
}

/**
 * @brief Aplica o filtro Sobel a uma imagem
 *
 * @param img Ponteiro para a imagem a ser filtrada
 */
void filtroSobel(Imagem *img)
{
    // Detalhes da Função:
    // Adiciona uma cópia para ler os vizinhos originais.
    // Define as matrizes de Sobel
    // Percorre a imagem ignorando as bordas.
    // Aplica o  gradienteX e gradienteY.
    // Combinação por magnitude.
    // Libera a memória da cópia.
    if (!img)
        return;

    Imagem *original = copiaImagem(img);
    if (!original)
        return;

    int largura = obtemLargura(img);
    int altura = obtemAltura(img);

    int gradienteX[3][3] = {
        {1, 0, -1},
        {2, 0, -2},
        {1, 0, -1}};

    int gradienteY[3][3] = {
        {1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1}};

    for (int y = 1; y < altura - 1; y++)
    {
        for (int x = 1; x < largura - 1; x++)
        {

            int somaX_Vermelho = 0, somaX_Verde = 0, somaX_Azul = 0;
            int somaY_Vermelho = 0, somaY_Verde = 0, somaY_Azul = 0;

            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    Cor vizinho = obtemCorPixel(original, y + i, x + j);

                    somaX_Vermelho += vizinho.r * gradienteX[i + 1][j + 1];
                    somaX_Verde += vizinho.g * gradienteX[i + 1][j + 1];
                    somaX_Azul += vizinho.b * gradienteX[i + 1][j + 1];

                    somaY_Vermelho += vizinho.r * gradienteY[i + 1][j + 1];
                    somaY_Verde += vizinho.g * gradienteY[i + 1][j + 1];
                    somaY_Azul += vizinho.b * gradienteY[i + 1][j + 1];
                }
            }

            int magnitudeVermelho = 
                (int)sqrt(somaX_Vermelho * somaX_Vermelho + somaY_Vermelho * somaY_Vermelho);

            int magnitudeVerde = 
                (int)sqrt(somaX_Verde * somaX_Verde + somaY_Verde * somaY_Verde);

            int magnitudeAzul = 
                (int)sqrt(somaX_Azul * somaX_Azul + somaY_Azul * somaY_Azul);

            Cor novaCor;
            novaCor.r = ajustaCor(magnitudeVermelho);
            novaCor.g = ajustaCor(magnitudeVerde);
            novaCor.b = ajustaCor(magnitudeAzul);

            recolorePixel(img, y, x, novaCor);
        }
    }

    liberaImagem(original);
}

/**
 * @brief Aplica o filtro Laplace a uma imagem
 *
 * @param img Ponteiro para a imagem a ser filtrada
 */
void deteccaoBordasLaplace(Imagem *img)
{

    // Detalhes da Função:
    // Faz detecção de bordas na imagem utilizando o operador de Laplace.
    // É criada uma cópia da imagem original, que é usada para leitura dos valores dos pixels.
    // O filtro é aplicado separadamente no RGB
    // Depois os valores calculados são ajustados para o intervalo.
    // O processamento ignora os pixels da borda da imagem.
    // A imagem original é modificada diretamente e a cópia auxiliar é liberada.

    if (!img)
        return;

    Imagem *original = copiaImagem(img);
    if (!original)
        return;

    int largura = obtemLargura(img);
    int altura = obtemAltura(img);

    int laplace[3][3] = {
        {0, -1, 0},
        {-1, 4, -1},
        {0, -1, 0}};

    for (int y = 1; y < altura - 1; y++)
    {
        for (int x = 1; x < largura - 1; x++)
        {

            int somaVermelho = 0, somaVerde = 0, somaAzul = 0;

            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    Cor vizinho = obtemCorPixel(original, y + i, x + j);
                    int peso = laplace[i + 1][j + 1];

                    somaVermelho += vizinho.r * peso;
                    somaVerde += vizinho.g * peso;
                    somaAzul += vizinho.b * peso;
                }
            }

            Cor novaCor;
            novaCor.r = ajustaCor(somaVermelho);
            novaCor.g = ajustaCor(somaVerde);
            novaCor.b = ajustaCor(somaAzul);

            recolorePixel(img, y, x, novaCor);
        }
    }

    liberaImagem(original);
}

/**
 * @brief Aplica um filtro personalizado a uma imagem
 *
 * @param img Ponteiro para a imagem a ser filtrada
 */
void meuFiltro(Imagem *img)
{
    // Detalhes da Função:
    // Obs: Nessa função por ser muito grande resolvi espalhar os comentários pra facilitar a leitura.

    if (!img)
        return;

    // Cria cópia para leitura (necessário para filtros de vizinhança)
    Imagem *copiaParaLeitura = copiaImagem(img);
    if (!copiaParaLeitura)
        return;

    int largura = obtemLargura(img);
    int altura = obtemAltura(img);

    // Coordenadas do centro da imagem para o efeito de Vinheta
    double centroX = largura / 2.0;
    double centroY = altura / 2.0;

    // Distância máxima do centro até um canto.
    double distanciaMaxima = sqrt(centroX * centroX + centroY * centroY);

    // Percorre a imagem
    for (int y = 1; y < altura - 1; y++)
    {
        for (int x = 1; x < largura - 1; x++)
        {

            int somaVermelho = 0, somaVerde = 0, somaAzul = 0;

            // Aplica a convolução (Matriz 3x3 - Média simples)
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    Cor pixelVizinho = obtemCorPixel(copiaParaLeitura, y + i, x + j);

                    somaVermelho += pixelVizinho.r;
                    somaVerde += pixelVizinho.g;
                    somaAzul += pixelVizinho.b;
                }
            }

            // Média dos vizinhos
            int mediaVermelho = somaVermelho / 9;
            int mediaVerde = somaVerde / 9;
            int mediaAzul = somaAzul / 9;

            Cor pixelOriginal = obtemCorPixel(copiaParaLeitura, y, x);

            // Mistura o original com blur pra dar um efeito de foco
            int vermelhoFinal = (int)(pixelOriginal.r * 0.7 + mediaVermelho * 0.3);
            int verdeFinal = (int)(pixelOriginal.g * 0.7 + mediaVerde * 0.3);
            int azulFinal = (int)(pixelOriginal.b * 0.7 + mediaAzul * 0.3);

            // Aquece a imagem aumentando vermelho/verde e reduzindo azul
            vermelhoFinal = (int)(vermelhoFinal * 1.15);
            verdeFinal = (int)(verdeFinal * 1.05);
            azulFinal = (int)(azulFinal * 0.90);

            // Calcula distância euclidiana do pixel atual ao centro
            double distanciaAoCentro = sqrt(pow(x - centroX, 2) + pow(y - centroY, 2));

            // Fator de escurecimento com 1.0 no centro, menor nas bordas
            double fatorEscurecimento = 
                1.0 - (0.6 * (pow(distanciaAoCentro / distanciaMaxima, 2)));

            // aplica a vinheta
            vermelhoFinal = (int)(vermelhoFinal * fatorEscurecimento);
            verdeFinal = (int)(verdeFinal * fatorEscurecimento);
            azulFinal = (int)(azulFinal * fatorEscurecimento);

            // Monta o pixel finall
            Cor novaCor;
            novaCor.r = ajustaCor(vermelhoFinal);
            novaCor.g = ajustaCor(verdeFinal);
            novaCor.b = ajustaCor(azulFinal);

            recolorePixel(img, y, x, novaCor);
        }
    }

    liberaImagem(copiaParaLeitura);
}