#include <stdio.h>  
#include <stdlib.h> 
#include <math.h>   

#include "Imagem.h"

/**
 * @brief Estrutura que representa uma imagem
 * 
 */
struct imagem {
    int largura;   /* Número de colunas (largura) da imagem em pixels */
    int altura;    /* Número de linhas (altura) da imagem em pixels */
    int maxval;    /* Valor máximo dos pixels (deixe sempre 255)*/
    Cor **pixel;   /* Matriz altura x largura de pixels com os níveis RGB */
};

/**
 * @brief Aloca uma imagem com as dimensões especificadas.
 * 
 * @param largura Número de colunas (largura) da imagem
 * @param altura Número de linhas (altura) da imagem
 * @return Imagem* Ponteiro para a imagem alocada
 */
Imagem *alocaImagem(int largura, int altura){
    AVISO(Imagem.c : Ainda não implementei a função 'alocaImagem'); // Retire esssa mensagem ao implementar a fução
    
    // Com você :)
    
    return NULL;
}

/**
 * @brief Libera a memória alocada para uma imagem.
 * 
 * @param img Ponteiro para a imagem a ser liberada.
 */
void liberaImagem(Imagem *img){
    AVISO(Imagem.c: Ainda não implementei a função 'liberaImagem'); //Retire esssa mensagem ao implementar a fução

    // Com você :)
   
}

/**
 * @brief Retorna a largura da imagem
 * 
 * @param img Imagem alvo
 * @return int Largura da imagem
 */
int obtemLargura(Imagem *img){
    return img->largura;
}

/**
 * @brief Retorna a altura da imagem
 * 
 * @param img Imagem alvo
 * @return int Altura da imagem
 */
int obtemAltura(Imagem *img){
    return img->altura;
}

/**
 * @brief Retorna o valor máximo dos pixels da imagem
 * 
 * @param img Imagem alvo
 * @return int Valor máximo dos pixels da imagem
 */
int obtemMaxValue(Imagem *img){
    return img->maxval;
}

/**
 * @brief Retorna a cor de um pixel específico da imagem
 * 
 * @param img Imagem alvo
 * @param l Linha do pixel
 * @param c Coluna do pixel
 * @return Cor Cor do pixel especificado
 */
Cor obtemCorPixel(Imagem *img, int l, int c){
    return img->pixel[l][c];
}

/**
 * @brief Copia uma imagem
 * 
 * @param origem Imagem de origem
 * @return Imagem* Ponteiro para a imagem copiada
 */
Imagem* copiaImagem(Imagem *origem){
    AVISO(Imagem.c: Ainda não implementei a função 'copiaImagem'); //Retire esssa mensagem ao implementar a fução

    // Lembre-se de verificar se a imagem de origem foi alocada (origem diferente de NULL).
    // Com você :)
    return NULL;
}

/**
 * @brief Recolore um pixel específico da imagem
 * 
 * @param img Imagem alvo
 * @param l Linha do pixel
 * @param c Coluna do pixel
 * @param cor Cor a ser aplicada ao pixel
 */
void recolorePixel(Imagem *img, int l, int c, Cor cor){
    if( img == NULL ) {
        ERRO(Imagem.c: Imagem não foi alocada);
        return;
    }
    if( l < 0 || l >= img->altura || c < 0 || c >= img->largura ) {
        printf("l: %d, c: %d, altura: %d, largura: %d\n", l, c, img->altura, img->largura);
        ERRO(Imagem.c: Coordenadas do pixel estao fora dos limites da imagem);
        exit(1);
    }
    img->pixel[l][c].r = cor.r;
    img->pixel[l][c].g = cor.g;
    img->pixel[l][c].b = cor.b;
}

void liberarBuffer(Byte *pixels, gpointer data) {
    if(pixels)
        g_free(pixels);
}

/**
 * @brief Converte uma Imagem para um GdkPixbuf.
 * 
 * @param img Imagem a ser convertida.
 * @return GdkPixbuf* Ponteiro para o GdkPixbuf criado.
 */
GdkPixbuf *imagemParaPixbuf(Imagem *img) {
    if( img == NULL ) {
        ERRO(Imagem.c: Imagem não foi alocada);
        return NULL;
    }

    Byte *pixels = mallocSafe(obtemLargura(img) * obtemAltura(img) * 3);

    for (int i = 0; i < obtemAltura(img); i++) {
        for (int j = 0; j < obtemLargura(img); j++) {
            int idx = (i * obtemLargura(img) + j) * 3;
            Cor cor = obtemCorPixel(img, i, j);
            pixels[idx] = cor.r;
            pixels[idx + 1] = cor.g;
            pixels[idx + 2] = cor.b;
        }
    }

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(
        pixels, GDK_COLORSPACE_RGB, FALSE, 8,
        obtemLargura(img), obtemAltura(img), obtemLargura(img) * 3,
        liberarBuffer, NULL);

    return pixbuf;
}

/**
 * @brief Converte um GdkPixbuf para uma Imagem.
 * 
 * @param pixbuf GdkPixbuf a ser convertida.
 * @return Imagem* Ponteiro para a imagem criada.
 */
Imagem *PixbufParaImagem(GdkPixbuf *pixbuf) {
    if (!pixbuf)
        return NULL;

    int largura = gdk_pixbuf_get_width(pixbuf);
    int altura = gdk_pixbuf_get_height(pixbuf);
    int canais = gdk_pixbuf_get_n_channels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    Byte *dados = gdk_pixbuf_get_pixels(pixbuf);

    if (canais < 3)
        return NULL; /* não é RGB */

    /* Aloca a estrutura Imagem */
    Imagem *img = mallocSafe(sizeof(Imagem));

    img->largura = largura;
    img->altura = altura;
    img->maxval = 255;

    /* Aloca a matriz de pixels (altura x largura) */
    img->pixel = malloc(altura * sizeof(Cor *));
    if (!img->pixel) {
        free(img);
        return NULL;
    }
    for (int i = 0; i < altura; i++) {
        img->pixel[i] = malloc(largura * sizeof(Cor));
        if (!img->pixel[i]) {
            /* Liberação em caso de erro */
            for (int k = 0; k < i; k++)
                free(img->pixel[k]);
            free(img->pixel);
            free(img);
            return NULL;
        }
    }

    /* Copia os pixels do GdkPixbuf para a Imagem */
    for (int y = 0; y < altura; y++) {
        Byte *linha = dados + y * rowstride;
        for (int x = 0; x < largura; x++) {
            Byte *p = linha + x * canais;
            img->pixel[y][x].r = p[0];
            img->pixel[y][x].g = p[1];
            img->pixel[y][x].b = p[2];
        }
    }
    return img;
}

/**
 * @brief Carrega uma imagem de um arquivo no formato PPM (P3).
 * 
 * @param nomeArquivo Nome do arquivo a ser carregado.
 * @return Imagem* Ponteiro para a imagem carregada.
 */
Imagem *carregaImagem(const char *nomeArquivo){
    char buff[16];
    Imagem *img;
    FILE *fp;
    int c;
    fp = fopen(nomeArquivo, "r");

    if (fp == NULL) {
        fprintf(stderr, "não foi possivel abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }
    if (!fgets(buff, sizeof(buff), fp)) {
        perror("stdin");
        exit(1);
    }

    if (buff[0] != 'P' || buff[1] != '3') {
        fprintf(stderr, "Formato da imagem invalido (deve ser 'P3')\n");
        exit(1);
    }
    c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n') ;
        c = getc(fp);
    }

    ungetc(c, fp); /* coloca ultimo caractere devolta no buffer */
    int largura, altura, threshold;
    if (fscanf(fp, "%d %d", &largura, &altura) != 2) {
        fprintf(stderr, "Tamanho da imagem invalido\n");
        exit(1);
    }

    if (fscanf(fp, "%d", &threshold) != 1) {
        fprintf(stderr, "Componente rgb invalido\n");
        exit(1);
    }

    while (fgetc(fp) != '\n') ;

    img = alocaImagem(largura, altura);
    if (!img) {
        fprintf(stderr, "Erro na alocacao da memoria\n");
        exit(1);
    }

    int r, g, b;
    for (int i = 0; i < img->altura; i++) {
        for (int j = 0; j < img->largura; j++) {
            c = fscanf(fp, "%d %d %d", &r, &g, &b);
            img->pixel[i][j].r = (Byte)r;  /* intensidade vermelho */
            img->pixel[i][j].g = (Byte)g;  /* intensidade verde */
            img->pixel[i][j].b = (Byte)b;  /* intensidade azul */
        }
    }
    fclose(fp);
    return img;
}

/**
 * @brief Salva uma imagem em um arquivo no formato PPM (P3).
 * 
 * @param img Ponteiro para a imagem a ser salva.
 * @param nomeArquivo Nome do arquivo onde a imagem será salva.
 */
void salvaImagem(Imagem *img, const char *nomeArquivo){
    if( img == NULL ) {
        ERRO(Imagem.c: Imagem não foi alocada);
        return;
    }
    FILE *arquivo;
    int c, rgb_comp_color;
    arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        fprintf(stderr, "salvaImagem: ERRO: arquivo '%s' não pode ser criado\n", nomeArquivo);
        return;
    }
    fprintf(arquivo, "P3\n");
    fprintf(arquivo, "#FotoXop - Estrutura de Dados I\n");
    fprintf(arquivo, "%d %d %d\n", img->largura, img->altura, img->maxval);
    for (int i = 0; i < img->altura; i++)
        for (int j = 0; j < img->largura; j++){
            fprintf(arquivo, "%d %d %d\n", (int)img->pixel[i][j].r, (int)img->pixel[i][j].g, (int)img->pixel[i][j].b);
        }
        
    fclose(arquivo);
}

