#ifndef __FILTRO_H
#define __FILTRO_H

#include "Imagem.h"
#include "Cor.h"

void escalaDeCinzaImagem(Imagem *img);

void filtroSobel(Imagem *img);

void deteccaoBordasLaplace(Imagem *img);

void meuFiltro(Imagem *img);

#endif /* __FILTRO_H */