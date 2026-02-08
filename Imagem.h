#ifndef __IMAGEM_H
#define __IMAGEM_H

#include <gdk-pixbuf/gdk-pixbuf.h>

#include "Util.h"
#include "Cor.h"

typedef struct imagem Imagem;

Imagem* alocaImagem(int largura, int altura);

void liberaImagem(Imagem *img);

int obtemLargura(Imagem *img);

int obtemAltura(Imagem *img);

int obtemMaxValue(Imagem *img);

Cor obtemCorPixel(Imagem *img, int l, int c);

void recolorePixel(Imagem *img, int l, int c, Cor cor);

GdkPixbuf *imagemParaPixbuf(Imagem *img);

Imagem *PixbufParaImagem(GdkPixbuf *pixbuf);

Imagem* copiaImagem(Imagem *origem);

Imagem *carregaImagem(const char *nomeArquivo);

void salvaImagem(Imagem *img, const char *nomeArquivo);

#endif /* __IMAGEM_H */