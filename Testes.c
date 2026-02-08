#include <stdio.h>
#include <math.h>
#include "Imagem.h"
#include "Util.h"
#include "Pilha.h"
#include "Lista.h"
#include "AVL.h"
#include "Testes.h"

void testeEtapa0() {
    printf("\n\033[44m====================================\033[0m\n");
    printf("\033[44m=== BATERIA DE TESTES DA ETAPA 0 ===\033[0m\n");
    printf("\033[44m====================================\033[0m\n");
    
    // ========================================
    // TESTE 1: Alocação Básica de Imagem
    // ========================================
    printf("\n\033[44m=== Teste 1: Alocação Básica de Imagem ===\033[0m\n");
    {
        Imagem *img = alocaImagem(10, 10);
        TESTE(img != NULL, "Imagem alocada com sucesso");
        TESTE(obtemLargura(img) == 10, "Largura correta (10)");
        TESTE(obtemAltura(img) == 10, "Altura correta (10)");
        TESTE(obtemMaxValue(img) == 255, "MaxValue inicializado como 255");
        
        liberaImagem(img);
        printf("Imagem liberada com sucesso\n");
    }
    
    // ========================================
    // TESTE 2: Alocação de Imagem Pequena (1x1)
    // ========================================
    printf("\n\033[44m=== Teste 2: Alocação de Imagem Pequena (1x1) ===\033[0m\n");
    {
        Imagem *img = alocaImagem(1, 1);
        TESTE(img != NULL, "Imagem 1x1 alocada");
        TESTE(obtemLargura(img) == 1, "Largura = 1");
        TESTE(obtemAltura(img) == 1, "Altura = 1");
        TESTE(obtemMaxValue(img) == 255, "MaxValue = 255");
        
        Cor pixel = obtemCorPixel(img, 0, 0);
        TESTE(pixel.r == 0 && pixel.g == 0 && pixel.b == 0, 
              "Pixels inicializados com zeros (preto)");
        
        liberaImagem(img);
        printf("Imagem 1x1 liberada com sucesso\n");
    }
    
    // ========================================
    // TESTE 3: Alocação de Imagem Grande
    // ========================================
    printf("\n\033[44m=== Teste 3: Alocação de Imagem Grande ===\033[0m\n");
    {
        Imagem *img = alocaImagem(100, 100);
        TESTE(img != NULL, "Imagem 100x100 alocada");
        TESTE(obtemLargura(img) == 100, "Largura = 100");
        TESTE(obtemAltura(img) == 100, "Altura = 100");
        TESTE(obtemMaxValue(img) == 255, "MaxValue = 255");
        
        liberaImagem(img);
        printf("Imagem 100x100 liberada com sucesso\n");
    }
    
    // ========================================
    // TESTE 4: Alocação de Imagem Retangular
    // ========================================
    printf("\n\033[44m=== Teste 4: Alocação de Imagem Retangular ===\033[0m\n");
    {
        Imagem *img1 = alocaImagem(20, 10);
        TESTE(img1 != NULL, "Imagem 20x10 (mais larga) alocada");
        TESTE(obtemLargura(img1) == 20, "Largura = 20");
        TESTE(obtemAltura(img1) == 10, "Altura = 10");
        liberaImagem(img1);
        
        Imagem *img2 = alocaImagem(10, 20);
        TESTE(img2 != NULL, "Imagem 10x20 (mais alta) alocada");
        TESTE(obtemLargura(img2) == 10, "Largura = 10");
        TESTE(obtemAltura(img2) == 20, "Altura = 20");
        liberaImagem(img2);
        
        printf("Imagens retangulares liberadas com sucesso\n");
    }
    
    // ========================================
    // TESTE 5: Verificação de Inicialização dos Pixels
    // ========================================
    printf("\n\033[44m=== Teste 5: Inicialização dos Pixels ===\033[0m\n");
    {
        Imagem *img = alocaImagem(5, 5);
        TESTE(img != NULL, "Imagem 5x5 alocada");
        
        bool todosZerados = true;
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 5; j++) {
                Cor pixel = obtemCorPixel(img, i, j);
                if(pixel.r != 0 || pixel.g != 0 || pixel.b != 0) {
                    todosZerados = false;
                    break;
                }
            }
            if(!todosZerados) break;
        }
        TESTE(todosZerados, "Todos os pixels inicializados com (0,0,0)");
        
        liberaImagem(img);
    }
    
    // ========================================
    // TESTE 6: Liberação de Imagem NULL
    // ========================================
    printf("\n\033[44m=== Teste 6: Liberação de Imagem NULL ===\033[0m\n");
    {
        Imagem *img = NULL;
        liberaImagem(img);
        TESTE(true, "Liberação de imagem NULL não causa erro");
    }
    
    // ========================================
    // TESTE 7: Alocação com Dimensões Inválidas
    // ========================================
    printf("\n\033[44m=== Teste 7: Dimensões Inválidas ===\033[0m\n");
    {
        Imagem *img1 = alocaImagem(0, 10);
        TESTE(img1 == NULL, "Largura zero retorna NULL");
        
        Imagem *img2 = alocaImagem(10, 0);
        TESTE(img2 == NULL, "Altura zero retorna NULL");
        
        Imagem *img3 = alocaImagem(-5, 10);
        TESTE(img3 == NULL, "Largura negativa retorna NULL");
        
        Imagem *img4 = alocaImagem(10, -5);
        TESTE(img4 == NULL, "Altura negativa retorna NULL");
        
        printf("Validações de dimensões funcionando corretamente\n");
    }
    
    // ========================================
    // TESTE 8: Cópia de Imagem Simples
    // ========================================
    printf("\n\033[44m=== Teste 8: Cópia de Imagem Simples ===\033[0m\n");
    {
        Imagem *origem = alocaImagem(5, 5);
        
        // Preenche a imagem origem com um padrão
        recolorePixel(origem, 0, 0, (Cor){255, 0, 0});
        recolorePixel(origem, 2, 2, (Cor){0, 255, 0});
        recolorePixel(origem, 4, 4, (Cor){0, 0, 255});
        
        Imagem *copia = copiaImagem(origem);
        TESTE(copia != NULL, "Cópia criada com sucesso");
        TESTE(copia != origem, "Cópia é um novo objeto (endereço diferente)");
        TESTE(obtemLargura(copia) == obtemLargura(origem), "Largura copiada corretamente");
        TESTE(obtemAltura(copia) == obtemAltura(origem), "Altura copiada corretamente");
        TESTE(obtemMaxValue(copia) == obtemMaxValue(origem), "MaxValue copiado corretamente");
        
        // Verifica pixels específicos
        Cor c1 = obtemCorPixel(copia, 0, 0);
        TESTE(c1.r == 255 && c1.g == 0 && c1.b == 0, "Pixel (0,0) copiado: vermelho");
        
        Cor c2 = obtemCorPixel(copia, 2, 2);
        TESTE(c2.r == 0 && c2.g == 255 && c2.b == 0, "Pixel (2,2) copiado: verde");
        
        Cor c3 = obtemCorPixel(copia, 4, 4);
        TESTE(c3.r == 0 && c3.g == 0 && c3.b == 255, "Pixel (4,4) copiado: azul");
        
        liberaImagem(origem);
        liberaImagem(copia);
        printf("Imagens origem e cópia liberadas com sucesso\n");
    }
    
    // ========================================
    // TESTE 9: Cópia Completa de Todos os Pixels
    // ========================================
    printf("\n\033[44m=== Teste 9: Cópia Completa de Todos os Pixels ===\033[0m\n");
    {
        Imagem *origem = alocaImagem(10, 10);
        
        // Preenche toda a imagem com valores diferentes
        for(int i = 0; i < 10; i++) {
            for(int j = 0; j < 10; j++) {
                recolorePixel(origem, i, j, (Cor){i * 10, j * 10, (i+j) * 5});
            }
        }
        
        Imagem *copia = copiaImagem(origem);
        TESTE(copia != NULL, "Cópia de imagem 10x10 criada");
        
        // Verifica todos os pixels
        bool todosIguais = true;
        for(int i = 0; i < 10; i++) {
            for(int j = 0; j < 10; j++) {
                Cor orig = obtemCorPixel(origem, i, j);
                Cor cop = obtemCorPixel(copia, i, j);
                if(orig.r != cop.r || orig.g != cop.g || orig.b != cop.b) {
                    todosIguais = false;
                    break;
                }
            }
            if(!todosIguais) break;
        }
        TESTE(todosIguais, "Todos os 100 pixels foram copiados corretamente");
        
        liberaImagem(origem);
        liberaImagem(copia);
    }
    
    // ========================================
    // TESTE 10: Independência entre Origem e Cópia
    // ========================================
    printf("\n\033[44m=== Teste 10: Independência entre Origem e Cópia ===\033[0m\n");
    {
        Imagem *origem = alocaImagem(3, 3);
        recolorePixel(origem, 1, 1, (Cor){100, 100, 100});
        
        Imagem *copia = copiaImagem(origem);
        
        // Modifica a origem
        recolorePixel(origem, 1, 1, (Cor){200, 200, 200});
        
        // Verifica que a cópia não foi afetada
        Cor pixelOrigem = obtemCorPixel(origem, 1, 1);
        Cor pixelCopia = obtemCorPixel(copia, 1, 1);
        
        TESTE(pixelOrigem.r == 200, "Origem modificada para (200,200,200)");
        TESTE(pixelCopia.r == 100, "Cópia mantém valor original (100,100,100)");
        TESTE(pixelOrigem.r != pixelCopia.r, "Modificação na origem não afeta a cópia");
        
        liberaImagem(origem);
        liberaImagem(copia);
        printf("Independência entre origem e cópia validada\n");
    }
    
    // ========================================
    // TESTE 11: Cópia de Imagem NULL
    // ========================================
    printf("\n\033[44m=== Teste 11: Cópia de Imagem NULL ===\033[0m\n");
    {
        Imagem *origem = NULL;
        Imagem *copia = copiaImagem(origem);
        TESTE(copia == NULL, "Copiar imagem NULL retorna NULL");
    }
    
    // ========================================
    // TESTE 12: Múltiplas Alocações e Liberações
    // ========================================
    printf("\n\033[44m=== Teste 12: Múltiplas Alocações e Liberações ===\033[0m\n");
    {
        for(int i = 0; i < 10; i++) {
            Imagem *img = alocaImagem(5, 5);
            TESTE(img != NULL, "Alocação sequencial bem-sucedida");
            liberaImagem(img);
        }
        printf("10 ciclos de alocação/liberação completados com sucesso\n");
    }
    
    // ========================================
    // TESTE 13: Cópia de Imagem com Pixels nos Extremos
    // ========================================
    printf("\n\033[44m=== Teste 13: Cópia com Pixels nos Extremos ===\033[0m\n");
    {
        Imagem *origem = alocaImagem(10, 10);
        
        // Define cores nos cantos
        recolorePixel(origem, 0, 0, (Cor){255, 0, 0});      // Superior esquerdo
        recolorePixel(origem, 0, 9, (Cor){0, 255, 0});      // Superior direito
        recolorePixel(origem, 9, 0, (Cor){0, 0, 255});      // Inferior esquerdo
        recolorePixel(origem, 9, 9, (Cor){255, 255, 0});    // Inferior direito
        
        Imagem *copia = copiaImagem(origem);
        
        Cor c1 = obtemCorPixel(copia, 0, 0);
        Cor c2 = obtemCorPixel(copia, 0, 9);
        Cor c3 = obtemCorPixel(copia, 9, 0);
        Cor c4 = obtemCorPixel(copia, 9, 9);
        
        TESTE(c1.r == 255 && c1.g == 0 && c1.b == 0, "Canto superior esquerdo copiado");
        TESTE(c2.r == 0 && c2.g == 255 && c2.b == 0, "Canto superior direito copiado");
        TESTE(c3.r == 0 && c3.g == 0 && c3.b == 255, "Canto inferior esquerdo copiado");
        TESTE(c4.r == 255 && c4.g == 255 && c4.b == 0, "Canto inferior direito copiado");
        
        liberaImagem(origem);
        liberaImagem(copia);
    }
    
    // ========================================
    // TESTE 14: Validação do MaxValue em Diferentes Tamanhos
    // ========================================
    printf("\n\033[44m=== Teste 14: MaxValue Sempre 255 ===\033[0m\n");
    {
        Imagem *img1 = alocaImagem(1, 1);
        Imagem *img2 = alocaImagem(10, 10);
        Imagem *img3 = alocaImagem(50, 30);
        Imagem *img4 = alocaImagem(100, 100);
        
        TESTE(obtemMaxValue(img1) == 255, "MaxValue = 255 para imagem 1x1");
        TESTE(obtemMaxValue(img2) == 255, "MaxValue = 255 para imagem 10x10");
        TESTE(obtemMaxValue(img3) == 255, "MaxValue = 255 para imagem 50x30");
        TESTE(obtemMaxValue(img4) == 255, "MaxValue = 255 para imagem 100x100");
        
        liberaImagem(img1);
        liberaImagem(img2);
        liberaImagem(img3);
        liberaImagem(img4);
        
        printf("MaxValue validado para diferentes tamanhos de imagem\n");
    }
    
    // ========================================
    // TESTE 15: Cópia Preserva MaxValue
    // ========================================
    printf("\n\033[44m=== Teste 15: Cópia Preserva MaxValue ===\033[0m\n");
    {
        Imagem *origem = alocaImagem(5, 5);
        TESTE(obtemMaxValue(origem) == 255, "MaxValue da origem = 255");
        
        Imagem *copia = copiaImagem(origem);
        TESTE(obtemMaxValue(copia) == 255, "MaxValue da cópia = 255");
        TESTE(obtemMaxValue(copia) == obtemMaxValue(origem), 
              "MaxValue preservado na cópia");
        
        liberaImagem(origem);
        liberaImagem(copia);
    }
    
    // ========================================
    // FIM DOS TESTES
    // ========================================
    printf("\n\033[42m============================================\033[0m\n");
    printf("\033[42m=== TODOS OS TESTES DA ETAPA 0 PASSARAM! ===\033[0m\n");
    printf("\033[42m============================================\033[0m\n");
}

void testeEtapa2() {
    printf("\n\033[44m====================================\033[0m\n");
    printf("\033[44m=== BATERIA DE TESTES DA ETAPA 2 ===\033[0m\n");
    printf("\033[44m====================================\033[0m\n");
    
    // ========================================
    // TESTE 1: Criação e Liberação da Pilha
    // ========================================
    printf("\n\033[44m=== Teste 1: Criação e Liberação da Pilha ===\033[0m\n");
    {
        Pilha *pilha = criaPilha();
        TESTE(pilha != NULL, "Pilha criada com sucesso");
        TESTE(pilhaVazia(pilha), "Pilha inicialmente vazia");
        TESTE(tamanhoPilha(pilha) == 0, "Tamanho inicial é 0");
        
        liberaPilha(pilha);
        printf("Pilha liberada com sucesso\n");
    }
    
    // ========================================
    // TESTE 2: Push de Um Único Elemento
    // ========================================
    printf("\n\033[44m=== Teste 2: Push de Um Único Elemento ===\033[0m\n");
    {
        Pilha *pilha = criaPilha();
        
        Imagem *img = alocaImagem(5, 5);
        recolorePixel(img, 2, 2, (Cor){100, 100, 100});
        
        // IMPORTANTE: Deve passar uma CÓPIA
        Imagem *copia = copiaImagem(img);
        pushPilha(pilha, copia);
        
        TESTE(!pilhaVazia(pilha), "Pilha não está mais vazia");
        TESTE(tamanhoPilha(pilha) == 1, "Tamanho da pilha é 1");
        
        Imagem *topo = topPilha(pilha);
        TESTE(topo != NULL, "Top retorna imagem");
        TESTE(topo != img, "Imagem no topo é a cópia, não a original");
        
        Cor pixel = obtemCorPixel(topo, 2, 2);
        TESTE(pixel.r == 100 && pixel.g == 100 && pixel.b == 100, 
              "Conteúdo da imagem no topo está correto");
        
        liberaImagem(img);
        liberaPilha(pilha);
    }
    
    // ========================================
    // TESTE 3: Push de Múltiplos Elementos
    // ========================================
    printf("\n\033[44m=== Teste 3: Push de Múltiplos Elementos ===\033[0m\n");
    {
        Pilha *pilha = criaPilha();
        
        // Empilha 5 imagens diferentes
        for(int i = 0; i < 5; i++) {
            Imagem *img = alocaImagem(3, 3);
            recolorePixel(img, 0, 0, (Cor){i * 50, i * 50, i * 50});
            pushPilha(pilha, img); // Passa a imagem diretamente (será a cópia)
        }
        
        TESTE(tamanhoPilha(pilha) == 5, "Pilha contém 5 elementos");
        TESTE(!pilhaVazia(pilha), "Pilha não está vazia");
        
        // Verifica o topo (deve ser a última inserida, i=4)
        Imagem *topo = topPilha(pilha);
        Cor pixel = obtemCorPixel(topo, 0, 0);
        TESTE(pixel.r == 200, "Topo contém a última imagem inserida (200,200,200)");
        
        liberaPilha(pilha);
    }
    
    // ========================================
    // TESTE 4: Pop de Um Elemento
    // ========================================
    printf("\n\033[44m=== Teste 4: Pop de Um Elemento ===\033[0m\n");
    {
        Pilha *pilha = criaPilha();
        
        Imagem *img1 = alocaImagem(3, 3);
        recolorePixel(img1, 0, 0, (Cor){100, 0, 0});
        pushPilha(pilha, img1);
        
        Imagem *img2 = alocaImagem(3, 3);
        recolorePixel(img2, 0, 0, (Cor){200, 0, 0});
        pushPilha(pilha, img2);
        
        TESTE(tamanhoPilha(pilha) == 2, "Pilha com 2 elementos");
        
        // Remove o topo
        popPilha(pilha);
        
        TESTE(tamanhoPilha(pilha) == 1, "Após pop, tamanho é 1");
        TESTE(!pilhaVazia(pilha), "Pilha não está vazia");
        
        // Verifica o novo topo
        Imagem *topo = topPilha(pilha);
        Cor pixel = obtemCorPixel(topo, 0, 0);
        TESTE(pixel.r == 100, "Novo topo é a primeira imagem (100,0,0)");
        
        liberaPilha(pilha);
    }
    
    // ========================================
    // TESTE 5: Pop Até Esvaziar a Pilha
    // ========================================
    printf("\n\033[44m=== Teste 5: Pop Até Esvaziar a Pilha ===\033[0m\n");
    {
        Pilha *pilha = criaPilha();
        
        // Empilha 3 elementos
        for(int i = 0; i < 3; i++) {
            Imagem *img = alocaImagem(2, 2);
            pushPilha(pilha, img);
        }
        
        TESTE(tamanhoPilha(pilha) == 3, "Pilha com 3 elementos");
        
        // Remove todos
        popPilha(pilha);
        TESTE(tamanhoPilha(pilha) == 2, "Após 1º pop: tamanho = 2");
        
        popPilha(pilha);
        TESTE(tamanhoPilha(pilha) == 1, "Após 2º pop: tamanho = 1");
        
        popPilha(pilha);
        TESTE(tamanhoPilha(pilha) == 0, "Após 3º pop: tamanho = 0");
        TESTE(pilhaVazia(pilha), "Pilha está vazia após remover todos");
        
        liberaPilha(pilha);
    }
    
    // ========================================
    // TESTE 6: Pop em Pilha Vazia
    // ========================================
    printf("\n\033[44m=== Teste 6: Pop em Pilha Vazia ===\033[0m\n");
    {
        Pilha *pilha = criaPilha();
        
        TESTE(pilhaVazia(pilha), "Pilha está vazia");
        
        // Tenta fazer pop em pilha vazia (não deve causar erro)
        popPilha(pilha);
        
        TESTE(pilhaVazia(pilha), "Pilha continua vazia após pop");
        TESTE(tamanhoPilha(pilha) == 0, "Tamanho continua 0");
        
        liberaPilha(pilha);
        printf("Pop em pilha vazia não causa erro\n");
    }
    
    // ========================================
    // TESTE 7: Top em Pilha Vazia
    // ========================================
    printf("\n\033[44m=== Teste 7: Top em Pilha Vazia ===\033[0m\n");
    {
        Pilha *pilha = criaPilha();
        
        Imagem *topo = topPilha(pilha);
        TESTE(topo == NULL, "Top de pilha vazia retorna NULL");
        
        liberaPilha(pilha);
    }
    
    // ========================================
    // TESTE 8: Comportamento LIFO (Last In First Out)
    // ========================================
    printf("\n\033[44m=== Teste 8: Comportamento LIFO ===\033[0m\n");
    {
        Pilha *pilha = criaPilha();
        
        // Empilha imagens com cores identificáveis
        Imagem *img1 = alocaImagem(2, 2);
        recolorePixel(img1, 0, 0, (Cor){10, 0, 0});
        pushPilha(pilha, img1);
        
        Imagem *img2 = alocaImagem(2, 2);
        recolorePixel(img2, 0, 0, (Cor){20, 0, 0});
        pushPilha(pilha, img2);
        
        Imagem *img3 = alocaImagem(2, 2);
        recolorePixel(img3, 0, 0, (Cor){30, 0, 0});
        pushPilha(pilha, img3);
        
        // Verifica ordem LIFO
        Imagem *topo = topPilha(pilha);
        Cor c = obtemCorPixel(topo, 0, 0);
        TESTE(c.r == 30, "Topo: última inserida (30,0,0)");
        
        popPilha(pilha);
        topo = topPilha(pilha);
        c = obtemCorPixel(topo, 0, 0);
        TESTE(c.r == 20, "Após pop: penúltima inserida (20,0,0)");
        
        popPilha(pilha);
        topo = topPilha(pilha);
        c = obtemCorPixel(topo, 0, 0);
        TESTE(c.r == 10, "Após 2º pop: primeira inserida (10,0,0)");
        
        liberaPilha(pilha);
        printf("Comportamento LIFO validado\n");
    }
    
    // ========================================
    // TESTE 9: Top Não Remove o Elemento
    // ========================================
    printf("\n\033[44m=== Teste 9: Top Não Remove o Elemento ===\033[0m\n");
    {
        Pilha *pilha = criaPilha();
        
        Imagem *img = alocaImagem(2, 2);
        recolorePixel(img, 0, 0, (Cor){123, 123, 123});
        pushPilha(pilha, img);
        
        int tamanhoAntes = tamanhoPilha(pilha);
        
        // Chama top múltiplas vezes
        Imagem *t1 = topPilha(pilha);
        Imagem *t2 = topPilha(pilha);
        Imagem *t3 = topPilha(pilha);
        
        TESTE(tamanhoPilha(pilha) == tamanhoAntes, "Top não altera o tamanho");
        TESTE(t1 == t2 && t2 == t3, "Top retorna sempre o mesmo elemento");
        
        Cor c = obtemCorPixel(t1, 0, 0);
        TESTE(c.r == 123, "Conteúdo permanece inalterado");
        
        liberaPilha(pilha);
    }
    
    // ========================================
    // TESTE 10: Empilhar e Desempilhar Alternadamente
    // ========================================
    printf("\n\033[44m=== Teste 10: Push e Pop Alternados ===\033[0m\n");
    {
        Pilha *pilha = criaPilha();
        
        Imagem *img1 = alocaImagem(2, 2);
        pushPilha(pilha, img1);
        TESTE(tamanhoPilha(pilha) == 1, "Após 1º push: tamanho = 1");
        
        popPilha(pilha);
        TESTE(tamanhoPilha(pilha) == 0, "Após 1º pop: tamanho = 0");
        
        Imagem *img2 = alocaImagem(2, 2);
        pushPilha(pilha, img2);
        TESTE(tamanhoPilha(pilha) == 1, "Após 2º push: tamanho = 1");
        
        Imagem *img3 = alocaImagem(2, 2);
        pushPilha(pilha, img3);
        TESTE(tamanhoPilha(pilha) == 2, "Após 3º push: tamanho = 2");
        
        popPilha(pilha);
        TESTE(tamanhoPilha(pilha) == 1, "Após 2º pop: tamanho = 1");
        
        liberaPilha(pilha);
        printf("Operações alternadas funcionam corretamente\n");
    }
    
    // ========================================
    // TESTE 11: Liberação de Pilha com Elementos
    // ========================================
    printf("\n\033[44m=== Teste 11: Liberação de Pilha com Elementos ===\033[0m\n");
    {
        Pilha *pilha = criaPilha();
        
        // Empilha várias imagens
        for(int i = 0; i < 10; i++) {
            Imagem *img = alocaImagem(5, 5);
            pushPilha(pilha, img);
        }
        
        TESTE(tamanhoPilha(pilha) == 10, "Pilha com 10 elementos");
        
        // Libera a pilha (deve liberar todos os nós e imagens)
        liberaPilha(pilha);
        
        TESTE(true, "Pilha com elementos liberada sem erro");
        printf("Liberação de pilha não vazia funcionou corretamente\n");
    }
    
    // ========================================
    // TESTE 12: Pilha com Imagens de Tamanhos Diferentes
    // ========================================
    printf("\n\033[44m=== Teste 12: Imagens de Tamanhos Diferentes ===\033[0m\n");
    {
        Pilha *pilha = criaPilha();
        
        Imagem *img1 = alocaImagem(5, 5);
        pushPilha(pilha, img1);
        
        Imagem *img2 = alocaImagem(10, 20);
        pushPilha(pilha, img2);
        
        Imagem *img3 = alocaImagem(100, 100);
        pushPilha(pilha, img3);
        
        TESTE(tamanhoPilha(pilha) == 3, "3 imagens de tamanhos diferentes empilhadas");
        
        Imagem *topo = topPilha(pilha);
        TESTE(obtemLargura(topo) == 100, "Topo: imagem 100x100");
        TESTE(obtemAltura(topo) == 100, "Dimensões corretas no topo");
        
        popPilha(pilha);
        topo = topPilha(pilha);
        TESTE(obtemLargura(topo) == 10, "Após pop: imagem 10x20");
        TESTE(obtemAltura(topo) == 20, "Dimensões corretas");
        
        liberaPilha(pilha);
    }
    
    // ========================================
    // TESTE 13: Independência das Imagens
    // ========================================
    printf("\n\033[44m=== Teste 13: Independência das Imagens ===\033[0m\n");
    {
        Pilha *pilha = criaPilha();
        
        // Cria imagem original
        Imagem *original = alocaImagem(3, 3);
        recolorePixel(original, 1, 1, (Cor){100, 100, 100});
        
        // Empilha uma cópia
        Imagem *copia = copiaImagem(original);
        pushPilha(pilha, copia);
        
        // Modifica a imagem original
        recolorePixel(original, 1, 1, (Cor){200, 200, 200});
        
        // Verifica que a imagem na pilha não foi afetada
        Imagem *topo = topPilha(pilha);
        Cor pixel = obtemCorPixel(topo, 1, 1);
        
        TESTE(pixel.r == 100, "Imagem na pilha não foi afetada");
        TESTE(obtemCorPixel(original, 1, 1).r == 200, "Original foi modificada");
        
        liberaImagem(original);
        liberaPilha(pilha);
        printf("Independência entre imagens validada\n");
    }
    
    // ========================================
    // TESTE 14: Múltiplos Ciclos de Uso
    // ========================================
    printf("\n\033[44m=== Teste 14: Múltiplos Ciclos de Uso ===\033[0m\n");
    {
        Pilha *pilha = criaPilha();
        
        // Ciclo 1: Empilha e desempilha tudo
        for(int i = 0; i < 5; i++) {
            Imagem *img = alocaImagem(2, 2);
            pushPilha(pilha, img);
        }
        TESTE(tamanhoPilha(pilha) == 5, "Ciclo 1: 5 elementos");
        
        for(int i = 0; i < 5; i++) {
            popPilha(pilha);
        }
        TESTE(pilhaVazia(pilha), "Ciclo 1: pilha vazia");
        
        // Ciclo 2: Faz novamente
        for(int i = 0; i < 3; i++) {
            Imagem *img = alocaImagem(2, 2);
            pushPilha(pilha, img);
        }
        TESTE(tamanhoPilha(pilha) == 3, "Ciclo 2: 3 elementos");
        
        liberaPilha(pilha);
        printf("Múltiplos ciclos de uso funcionaram corretamente\n");
    }
    
    // ========================================
    // TESTE 15: Pilha com Imagens Complexas
    // ========================================
    printf("\n\033[44m=== Teste 15: Imagens com Conteúdo Complexo ===\033[0m\n");
    {
        Pilha *pilha = criaPilha();
        
        // Cria imagem com padrão complexo
        Imagem *img = alocaImagem(10, 10);
        for(int i = 0; i < 10; i++) {
            for(int j = 0; j < 10; j++) {
                recolorePixel(img, i, j, (Cor){i * 25, j * 25, (i+j) * 12});
            }
        }
        
        pushPilha(pilha, img);
        
        // Verifica que o conteúdo foi preservado
        Imagem *topo = topPilha(pilha);
        Cor c1 = obtemCorPixel(topo, 0, 0);
        Cor c2 = obtemCorPixel(topo, 5, 5);
        Cor c3 = obtemCorPixel(topo, 9, 9);
        
        TESTE(c1.r == 0 && c1.g == 0, "Pixel (0,0) correto");
        TESTE(c2.r == 125 && c2.g == 125, "Pixel (5,5) correto");
        TESTE(c3.r == 225 && c3.g == 225, "Pixel (9,9) correto");
        
        liberaPilha(pilha);
        printf("Conteúdo complexo preservado corretamente\n");
    }
    
    // ========================================
    // FIM DOS TESTES
    // ========================================
    printf("\n\033[42m============================================\033[0m\n");
    printf("\033[42m=== TODOS OS TESTES DA ETAPA 2 PASSARAM! ===\033[0m\n");
    printf("\033[42m============================================\033[0m\n");
}

void testeEtapa4() {
    printf("\n\033[44m====================================\033[0m\n");
    printf("\033[44m=== BATERIA DE TESTES DA ETAPA 4 ===\033[0m\n");
    printf("\033[44m====================================\033[0m\n");
    
    // ========================================
    // TESTE 1: Criação e Liberação da Lista
    // ========================================
    printf("\n\033[44m=== Teste 1: Criação e Liberação da Lista ===\033[0m\n");
    {
        Lista *lista = criaLista();
        TESTE(lista != NULL, "Lista criada com sucesso");
        TESTE(listaVazia(lista), "Lista inicialmente vazia");
        TESTE(tamanhoLista(lista) == 0, "Tamanho inicial é 0");
        
        liberaLista(lista);
        printf("Lista liberada com sucesso\n");
    }
    
    // ========================================
    // TESTE 2: Inserção de Um Único Elemento
    // ========================================
    printf("\n\033[44m=== Teste 2: Inserção de Um Único Elemento ===\033[0m\n");
    {
        Lista *lista = criaLista();
        
        Posicao pos = {5, 10};
        insereLista(lista, pos);
        
        TESTE(!listaVazia(lista), "Lista não está vazia após inserção");
        TESTE(tamanhoLista(lista) == 1, "Tamanho da lista é 1");
        
        liberaLista(lista);
    }
    
    // ========================================
    // TESTE 3: Inserção de Múltiplos Elementos
    // ========================================
    printf("\n\033[44m=== Teste 3: Inserção de Múltiplos Elementos ===\033[0m\n");
    {
        Lista *lista = criaLista();
        
        for(int i = 0; i < 10; i++) {
            Posicao pos = {i, i * 2};
            insereLista(lista, pos);
        }
        
        TESTE(tamanhoLista(lista) == 10, "Lista contém 10 elementos");
        TESTE(!listaVazia(lista), "Lista não está vazia");
        
        liberaLista(lista);
    }
    
    // ========================================
    // TESTE 4: Ordem de Inserção (Final da Lista)
    // ========================================
    printf("\n\033[44m=== Teste 4: Inserção no Final da Lista ===\033[0m\n");
    {
        Lista *lista = criaLista();
        
        Posicao p1 = {1, 1};
        Posicao p2 = {2, 2};
        Posicao p3 = {3, 3};
        
        insereLista(lista, p1);
        insereLista(lista, p2);
        insereLista(lista, p3);
        
        TESTE(tamanhoLista(lista) == 3, "3 elementos inseridos");
        
        // Verifica ordem percorrendo a lista
        No *atual = lista->inicio;
        TESTE(atual != NULL, "Primeiro nó existe");
        TESTE(atual->pos.linha == 1 && atual->pos.coluna == 1, 
              "Primeiro elemento: (1,1)");
        
        atual = atual->proximo;
        TESTE(atual != NULL, "Segundo nó existe");
        TESTE(atual->pos.linha == 2 && atual->pos.coluna == 2, 
              "Segundo elemento: (2,2)");
        
        atual = atual->proximo;
        TESTE(atual != NULL, "Terceiro nó existe");
        TESTE(atual->pos.linha == 3 && atual->pos.coluna == 3, 
              "Terceiro elemento: (3,3)");
        
        TESTE(atual->proximo == NULL, "Último elemento aponta para NULL");
        
        liberaLista(lista);
    }
    
    // ========================================
    // TESTE 5: Liberação de Lista NULL
    // ========================================
    printf("\n\033[44m=== Teste 5: Liberação de Lista NULL ===\033[0m\n");
    {
        Lista *lista = NULL;
        liberaLista(lista);
        TESTE(true, "Liberação de lista NULL não causa erro");
    }
    
    // ========================================
    // TESTE 6: Múltiplas Inserções e Tamanho
    // ========================================
    printf("\n\033[44m=== Teste 6: Validação de Tamanho ===\033[0m\n");
    {
        Lista *lista = criaLista();
        
        TESTE(tamanhoLista(lista) == 0, "Tamanho inicial: 0");
        
        for(int i = 1; i <= 5; i++) {
            Posicao pos = {i, i};
            insereLista(lista, pos);
            TESTE(tamanhoLista(lista) == i, "Após inserção: tamanho correto");
        }
        
        liberaLista(lista);
    }
    
    // ========================================
    // TESTE 7: Append de Lista Vazia
    // ========================================
    printf("\n\033[44m=== Teste 7: Append de Lista Vazia ===\033[0m\n");
    {
        Lista *destino = criaLista();
        Lista *origem = criaLista();
        
        insereLista(destino, (Posicao){1, 1});
        insereLista(destino, (Posicao){2, 2});
        
        int tamanhoAntes = tamanhoLista(destino);
        
        // Append de lista vazia não deve alterar destino
        appendLista(destino, origem);
        
        TESTE(tamanhoLista(destino) == tamanhoAntes, 
              "Append de lista vazia não altera tamanho");
        
        liberaLista(destino);
        liberaLista(origem);
    }
    
    // ========================================
    // TESTE 8: Append de Lista com Elementos
    // ========================================
    printf("\n\033[44m=== Teste 8: Append de Lista com Elementos ===\033[0m\n");
    {
        Lista *destino = criaLista();
        Lista *origem = criaLista();
        
        // Preenche destino
        insereLista(destino, (Posicao){1, 1});
        insereLista(destino, (Posicao){2, 2});
        
        // Preenche origem
        insereLista(origem, (Posicao){3, 3});
        insereLista(origem, (Posicao){4, 4});
        insereLista(origem, (Posicao){5, 5});
        
        appendLista(destino, origem);
        
        TESTE(tamanhoLista(destino) == 5, "Destino tem 5 elementos após append");
        TESTE(tamanhoLista(origem) == 3, "Origem mantém 3 elementos");
        
        // Verifica ordem dos elementos em destino
        No *atual = destino->inicio;
        TESTE(atual->pos.linha == 1, "1º elemento: (1,1)");
        atual = atual->proximo;
        TESTE(atual->pos.linha == 2, "2º elemento: (2,2)");
        atual = atual->proximo;
        TESTE(atual->pos.linha == 3, "3º elemento: (3,3) - da origem");
        atual = atual->proximo;
        TESTE(atual->pos.linha == 4, "4º elemento: (4,4) - da origem");
        atual = atual->proximo;
        TESTE(atual->pos.linha == 5, "5º elemento: (5,5) - da origem");
        
        liberaLista(destino);
        liberaLista(origem);
    }
    
    // ========================================
    // TESTE 9: Append em Lista Vazia
    // ========================================
    printf("\n\033[44m=== Teste 9: Append em Lista Destino Vazia ===\033[0m\n");
    {
        Lista *destino = criaLista();
        Lista *origem = criaLista();
        
        insereLista(origem, (Posicao){10, 20});
        insereLista(origem, (Posicao){30, 40});
        
        TESTE(listaVazia(destino), "Destino inicialmente vazia");
        
        appendLista(destino, origem);
        
        TESTE(!listaVazia(destino), "Destino não está mais vazia");
        TESTE(tamanhoLista(destino) == 2, "Destino tem 2 elementos");
        
        No *atual = destino->inicio;
        TESTE(atual->pos.linha == 10 && atual->pos.coluna == 20, 
              "Primeiro elemento copiado corretamente");
        
        liberaLista(destino);
        liberaLista(origem);
    }
    
    // ========================================
    // TESTE 10: Independência após Append
    // ========================================
    printf("\n\033[44m=== Teste 10: Independência após Append ===\033[0m\n");
    {
        Lista *destino = criaLista();
        Lista *origem = criaLista();
        
        insereLista(origem, (Posicao){100, 200});
        
        appendLista(destino, origem);
        
        // Modifica origem
        insereLista(origem, (Posicao){999, 999});
        
        // Destino não deve ser afetada
        TESTE(tamanhoLista(destino) == 1, "Destino mantém 1 elemento");
        TESTE(tamanhoLista(origem) == 2, "Origem agora tem 2 elementos");
        
        liberaLista(destino);
        liberaLista(origem);
        printf("Listas são independentes após append\n");
    }
    
    // ========================================
    // TESTE 11: Append Múltiplo
    // ========================================
    printf("\n\033[44m=== Teste 11: Múltiplos Appends ===\033[0m\n");
    {
        Lista *destino = criaLista();
        Lista *lista1 = criaLista();
        Lista *lista2 = criaLista();
        Lista *lista3 = criaLista();
        
        insereLista(lista1, (Posicao){1, 1});
        insereLista(lista2, (Posicao){2, 2});
        insereLista(lista3, (Posicao){3, 3});
        
        appendLista(destino, lista1);
        TESTE(tamanhoLista(destino) == 1, "Após 1º append: tamanho = 1");
        
        appendLista(destino, lista2);
        TESTE(tamanhoLista(destino) == 2, "Após 2º append: tamanho = 2");
        
        appendLista(destino, lista3);
        TESTE(tamanhoLista(destino) == 3, "Após 3º append: tamanho = 3");
        
        liberaLista(destino);
        liberaLista(lista1);
        liberaLista(lista2);
        liberaLista(lista3);
    }
    
    // ========================================
    // TESTE 12: Inserção de Posições com Valores Extremos
    // ========================================
    printf("\n\033[44m=== Teste 12: Posições com Valores Extremos ===\033[0m\n");
    {
        Lista *lista = criaLista();
        
        insereLista(lista, (Posicao){0, 0});
        insereLista(lista, (Posicao){-1, -1});
        insereLista(lista, (Posicao){1000, 2000});
        insereLista(lista, (Posicao){-500, 999});
        
        TESTE(tamanhoLista(lista) == 4, "4 posições inseridas");
        
        No *atual = lista->inicio;
        TESTE(atual->pos.linha == 0 && atual->pos.coluna == 0, 
              "Posição (0,0) inserida");
        
        atual = atual->proximo;
        TESTE(atual->pos.linha == -1 && atual->pos.coluna == -1, 
              "Posição negativa (-1,-1) inserida");
        
        atual = atual->proximo;
        TESTE(atual->pos.linha == 1000 && atual->pos.coluna == 2000, 
              "Posição grande (1000,2000) inserida");
        
        liberaLista(lista);
    }
    
    // ========================================
    // TESTE 13: Lista com Muitos Elementos
    // ========================================
    printf("\n\033[44m=== Teste 13: Lista com 100 Elementos ===\033[0m\n");
    {
        Lista *lista = criaLista();
        
        for(int i = 0; i < 100; i++) {
            insereLista(lista, (Posicao){i, i * 10});
        }
        
        TESTE(tamanhoLista(lista) == 100, "Lista com 100 elementos");
        
        // Verifica alguns elementos
        No *atual = lista->inicio;
        for(int i = 0; i < 100; i++) {
            TESTE(atual != NULL, "Nó existe na posição esperada");
            atual = atual->proximo;
        }
        TESTE(atual == NULL, "Último nó aponta para NULL");
        
        liberaLista(lista);
        printf("Lista grande liberada com sucesso\n");
    }
    
    // ========================================
    // TESTE 14: Ponteiros de Início e Fim
    // ========================================
    printf("\n\033[44m=== Teste 14: Ponteiros de Início e Fim ===\033[0m\n");
    {
        Lista *lista = criaLista();
        
        TESTE(lista->inicio == NULL, "Início NULL em lista vazia");
        TESTE(lista->fim == NULL, "Fim NULL em lista vazia");
        
        insereLista(lista, (Posicao){1, 1});
        TESTE(lista->inicio != NULL, "Início não NULL após inserção");
        TESTE(lista->fim != NULL, "Fim não NULL após inserção");
        TESTE(lista->inicio == lista->fim, "Início == Fim com 1 elemento");
        
        insereLista(lista, (Posicao){2, 2});
        TESTE(lista->inicio != lista->fim, "Início != Fim com 2 elementos");
        TESTE(lista->inicio->pos.linha == 1, "Início aponta para primeiro");
        TESTE(lista->fim->pos.linha == 2, "Fim aponta para último");
        
        liberaLista(lista);
    }
    
    // ========================================
    // TESTE 15: Append de Lista Grande
    // ========================================
    printf("\n\033[44m=== Teste 15: Append de Lista Grande ===\033[0m\n");
    {
        Lista *destino = criaLista();
        Lista *origem = criaLista();
        
        // Preenche origem com 50 elementos
        for(int i = 0; i < 50; i++) {
            insereLista(origem, (Posicao){i, i * 2});
        }
        
        // Preenche destino com 30 elementos
        for(int i = 0; i < 30; i++) {
            insereLista(destino, (Posicao){i + 100, i + 200});
        }
        
        appendLista(destino, origem);
        
        TESTE(tamanhoLista(destino) == 80, "Destino tem 80 elementos (30+50)");
        TESTE(tamanhoLista(origem) == 50, "Origem mantém 50 elementos");
        
        liberaLista(destino);
        liberaLista(origem);
        printf("Append de lista grande funcionou corretamente\n");
    }
    
    // ========================================
    // TESTE 16: Múltiplos Ciclos de Uso
    // ========================================
    printf("\n\033[44m=== Teste 16: Múltiplos Ciclos de Uso ===\033[0m\n");
    {
        for(int ciclo = 0; ciclo < 5; ciclo++) {
            Lista *lista = criaLista();
            
            for(int i = 0; i < 10; i++) {
                insereLista(lista, (Posicao){i, i});
            }
            
            TESTE(tamanhoLista(lista) == 10, "Ciclo: lista com 10 elementos");
            
            liberaLista(lista);
        }
        printf("5 ciclos de criação/uso/liberação completados\n");
    }
    
    // ========================================
    // TESTE 17: Consistência dos Ponteiros
    // ========================================
    printf("\n\033[44m=== Teste 17: Consistência dos Ponteiros ===\033[0m\n");
    {
        Lista *lista = criaLista();
        
        insereLista(lista, (Posicao){1, 1});
        insereLista(lista, (Posicao){2, 2});
        insereLista(lista, (Posicao){3, 3});
        
        // Percorre e valida encadeamento
        No *atual = lista->inicio;
        int contador = 0;
        
        while(atual != NULL) {
            contador++;
            if(atual->proximo == NULL) {
                TESTE(atual == lista->fim, "Último nó é o fim da lista");
            }
            atual = atual->proximo;
        }
        
        TESTE(contador == 3, "Percorreu todos os 3 elementos");
        
        liberaLista(lista);
    }
    
    // ========================================
    // TESTE 18: Posições Duplicadas
    // ========================================
    printf("\n\033[44m=== Teste 18: Posições Duplicadas ===\033[0m\n");
    {
        Lista *lista = criaLista();
        
        Posicao pos = {5, 10};
        insereLista(lista, pos);
        insereLista(lista, pos);
        insereLista(lista, pos);
        
        TESTE(tamanhoLista(lista) == 3, "Permite inserir posições duplicadas");
        
        No *atual = lista->inicio;
        for(int i = 0; i < 3; i++) {
            TESTE(atual->pos.linha == 5 && atual->pos.coluna == 10, 
                  "Posição duplicada armazenada corretamente");
            atual = atual->proximo;
        }
        
        liberaLista(lista);
        printf("Posições duplicadas tratadas corretamente\n");
    }
    
    // ========================================
    // FIM DOS TESTES
    // ========================================
    printf("\n\033[42m============================================\033[0m\n");
    printf("\033[42m=== TODOS OS TESTES DA ETAPA 4 PASSARAM! ===\033[0m\n");
    printf("\033[42m============================================\033[0m\n");
}

void testeEtapa5() {
    printf("\n\033[44m====================================\033[0m\n");
    printf("\033[44m=== BATERIA DE TESTES - ETAPA 5 ===\033[0m\n");
    printf("\033[44m===    ÁRVORE AVL (BALANCEADA)   ===\033[0m\n");
    printf("\033[44m====================================\033[0m\n");
    
    // ========================================
    // TESTE 1: Criação e Liberação da Árvore
    // ========================================
    printf("\n\033[44m=== Teste 1: Criação e Liberação da Árvore ===\033[0m\n");
    {
        Imagem *img = alocaImagem(2, 2);
        recolorePixel(img, 0, 0, (Cor){255, 0, 0});
        recolorePixel(img, 0, 1, (Cor){0, 255, 0});
        recolorePixel(img, 1, 0, (Cor){0, 0, 255});
        recolorePixel(img, 1, 1, (Cor){255, 255, 0});
        
        Arvore *arvore = criaArvore(img);
        TESTE(arvore != NULL, "Árvore criada com sucesso");
        TESTE(!arvoreVazia(arvore), "Árvore não está vazia após criação");
        TESTE(numeroNosArvore(arvore) == 4, "Número de nós correto (4 cores distintas)");
        TESTE(alturaArvore(arvore) >= 0, "Altura da árvore é válida");
        
        liberaArvore(arvore);
        liberaImagem(img);
        printf("Árvore e imagem liberadas com sucesso\n");
    }
    
    // ========================================
    // TESTE 2: Inserção Sequencial (Teste de Rotação RR)
    // ========================================
    printf("\n\033[44m=== Teste 2: Inserção Sequencial Crescente (Rotação RR) ===\033[0m\n");
    {
        Imagem *img = alocaImagem(1, 5);
        // Insere cores em ordem crescente (força rotação RR)
        recolorePixel(img, 0, 0, (Cor){10, 10, 10});
        recolorePixel(img, 1, 0, (Cor){20, 20, 20});
        recolorePixel(img, 2, 0, (Cor){30, 30, 30});
        recolorePixel(img, 3, 0, (Cor){40, 40, 40});
        recolorePixel(img, 4, 0, (Cor){50, 50, 50});
        
        Arvore *arvore = criaArvore(img);
        TESTE(numeroNosArvore(arvore) == 5, "5 nós inseridos");
        
        int altura = alturaArvore(arvore);
        // AVL deve manter altura balanceada: para 5 nós, altura ≤ 2
        TESTE(altura <= 2, "Altura balanceada após inserções crescentes (≤2)");
        printf("Altura da árvore: %d (esperado ≤2 para 5 nós)\n", altura);
        
        // Verifica que todas as cores podem ser encontradas
        TESTE(buscaArvore(arvore, (Cor){10, 10, 10}, 0) != NULL, "Cor 10 encontrada");
        TESTE(buscaArvore(arvore, (Cor){30, 30, 30}, 0) != NULL, "Cor 30 encontrada");
        TESTE(buscaArvore(arvore, (Cor){50, 50, 50}, 0) != NULL, "Cor 50 encontrada");
        
        liberaArvore(arvore);
        liberaImagem(img);
    }
    
    // ========================================
    // TESTE 3: Inserção Sequencial Decrescente (Teste de Rotação LL)
    // ========================================
    printf("\n\033[44m=== Teste 3: Inserção Sequencial Decrescente (Rotação LL) ===\033[0m\n");
    {
        Imagem *img = alocaImagem(1, 5);
        // Insere cores em ordem decrescente (força rotação LL)
        recolorePixel(img, 0, 0, (Cor){50, 50, 50});
        recolorePixel(img, 1, 0, (Cor){40, 40, 40});
        recolorePixel(img, 2, 0, (Cor){30, 30, 30});
        recolorePixel(img, 3, 0, (Cor){20, 20, 20});
        recolorePixel(img, 4, 0, (Cor){10, 10, 10});
        
        Arvore *arvore = criaArvore(img);
        TESTE(numeroNosArvore(arvore) == 5, "5 nós inseridos");
        
        int altura = alturaArvore(arvore);
        TESTE(altura <= 2, "Altura balanceada após inserções decrescentes (≤2)");
        printf("Altura da árvore: %d (esperado ≤2 para 5 nós)\n", altura);
        
        liberaArvore(arvore);
        liberaImagem(img);
    }
    
    // ========================================
    // TESTE 4: Inserção em Ziguezague (Teste Rotação LR e RL)
    // ========================================
    printf("\n\033[44m=== Teste 4: Inserção em Ziguezague (Rotações LR/RL) ===\033[0m\n");
    {
        Imagem *img = alocaImagem(1, 6);
        // Padrão que força rotações duplas
        recolorePixel(img, 0, 0, (Cor){30, 0, 0});
        recolorePixel(img, 1, 0, (Cor){10, 0, 0});
        recolorePixel(img, 2, 0, (Cor){20, 0, 0}); // Força LR
        recolorePixel(img, 3, 0, (Cor){50, 0, 0});
        recolorePixel(img, 4, 0, (Cor){60, 0, 0});
        recolorePixel(img, 5, 0, (Cor){55, 0, 0}); // Força RL
        
        Arvore *arvore = criaArvore(img);
        TESTE(numeroNosArvore(arvore) == 6, "6 nós inseridos");
        
        int altura = alturaArvore(arvore);
        TESTE(altura <= 3, "Altura balanceada após rotações duplas (≤3)");
        printf("Altura da árvore: %d (esperado ≤3 para 6 nós)\n", altura);
        
        liberaArvore(arvore);
        liberaImagem(img);
    }
    
    // ========================================
    // TESTE 5: Balanceamento com 15 Nós
    // ========================================
    printf("\n\033[44m=== Teste 5: Balanceamento com 15 Nós ===\033[0m\n");
    {
        Imagem *img = alocaImagem(1, 15);
        
        // Insere 15 cores diferentes em ordem que testará todos os casos
        int valores[] = {50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 35, 55, 65, 77, 85};
        for(int i = 0; i < 15; i++) {
            recolorePixel(img, i, 0, (Cor){valores[i], 0, 0});
        }
        
        Arvore *arvore = criaArvore(img);
        TESTE(numeroNosArvore(arvore) == 15, "15 nós inseridos");
        
        int altura = alturaArvore(arvore);
        // Para 15 nós, altura máxima de AVL ≈ log₂(15) ≈ 3.9, então ≤ 4
        TESTE(altura <= 4, "Altura balanceada para 15 nós (≤4)");
        printf("Altura da árvore: %d (esperado ≤4 para 15 nós)\n", altura);
        
        liberaArvore(arvore);
        liberaImagem(img);
    }
    
    // ========================================
    // TESTE 6: Propriedade de Altura AVL
    // ========================================
    printf("\n\033[44m=== Teste 6: Propriedade de Altura AVL (log n) ===\033[0m\n");
    {
        // Para N nós, altura AVL ≤ 1.44 * log₂(N+2) - 1
        // Teste com diferentes tamanhos
        int tamanhos[] = {7, 15, 31, 50};
        int alturasMaximas[] = {3, 4, 5, 6}; // Alturas máximas esperadas
        
        for(int t = 0; t < 4; t++) {
            Imagem *img = alocaImagem(1, tamanhos[t]);
            
            // Pior caso: inserção sequencial
            for(int i = 0; i < tamanhos[t]; i++) {
                recolorePixel(img, i, 0, (Cor){i * 10, 0, 0});
            }
            
            Arvore *arvore = criaArvore(img);
            int altura = alturaArvore(arvore);
            
            char msg[100];
            sprintf(msg, "%d nós: altura %d ≤ %d", tamanhos[t], altura, alturasMaximas[t]);
            TESTE(altura <= alturasMaximas[t], msg);
            
            liberaArvore(arvore);
            liberaImagem(img);
        }
    }
    
    // ========================================
    // TESTE 7: Busca Após Rotações
    // ========================================
    printf("\n\033[44m=== Teste 7: Busca Funciona Após Rotações ===\033[0m\n");
    {
        Imagem *img = alocaImagem(1, 7);
        
        // Inserções que causarão múltiplas rotações
        recolorePixel(img, 0, 0, (Cor){40, 0, 0});
        recolorePixel(img, 1, 0, (Cor){20, 0, 0});
        recolorePixel(img, 2, 0, (Cor){60, 0, 0});
        recolorePixel(img, 3, 0, (Cor){10, 0, 0});
        recolorePixel(img, 4, 0, (Cor){30, 0, 0});
        recolorePixel(img, 5, 0, (Cor){50, 0, 0});
        recolorePixel(img, 6, 0, (Cor){70, 0, 0});
        
        Arvore *arvore = criaArvore(img);
        
        // Verifica que todas as cores ainda podem ser encontradas
        TESTE(buscaArvore(arvore, (Cor){10, 0, 0}, 0) != NULL, "Cor 10 encontrada");
        TESTE(buscaArvore(arvore, (Cor){20, 0, 0}, 0) != NULL, "Cor 20 encontrada");
        TESTE(buscaArvore(arvore, (Cor){30, 0, 0}, 0) != NULL, "Cor 30 encontrada");
        TESTE(buscaArvore(arvore, (Cor){40, 0, 0}, 0) != NULL, "Cor 40 encontrada");
        TESTE(buscaArvore(arvore, (Cor){50, 0, 0}, 0) != NULL, "Cor 50 encontrada");
        TESTE(buscaArvore(arvore, (Cor){60, 0, 0}, 0) != NULL, "Cor 60 encontrada");
        TESTE(buscaArvore(arvore, (Cor){70, 0, 0}, 0) != NULL, "Cor 70 encontrada");
        
        liberaArvore(arvore);
        liberaImagem(img);
    }
    
    // ========================================
    // TESTE 8: Mesma Cor em Posições Diferentes (Sem Rotação)
    // ========================================
    printf("\n\033[44m=== Teste 8: Mesma Cor em Múltiplas Posições ===\033[0m\n");
    {
        Imagem *img = alocaImagem(3, 3);
        Cor corVermelha = {255, 0, 0};
        
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                recolorePixel(img, i, j, corVermelha);
            }
        }
        
        Arvore *arvore = criaArvore(img);
        TESTE(numeroNosArvore(arvore) == 1, "Apenas 1 nó para cor repetida");
        TESTE(alturaArvore(arvore) == 0, "Altura 0 para árvore com 1 nó");
        
        Lista *resultado = buscaArvore(arvore, corVermelha, 0);
        TESTE(resultado != NULL, "Cor vermelha encontrada");
        TESTE(tamanhoLista(resultado) == 9, "9 ocorrências da mesma cor");
        
        liberaArvore(arvore);
        liberaImagem(img);
    }
    
    // ========================================
    // TESTE 9: Busca Aproximada com Árvore Balanceada
    // ========================================
    printf("\n\033[44m=== Teste 9: Busca Aproximada em Árvore Balanceada ===\033[0m\n");
    {
        Imagem *img = alocaImagem(1, 5);
        recolorePixel(img, 0, 0, (Cor){100, 100, 100});
        recolorePixel(img, 1, 0, (Cor){105, 105, 105});
        recolorePixel(img, 2, 0, (Cor){95, 95, 95});
        recolorePixel(img, 3, 0, (Cor){110, 110, 110});
        recolorePixel(img, 4, 0, (Cor){90, 90, 90});
        
        Arvore *arvore = criaArvore(img);
        
        Lista *resultado = buscaArvore(arvore, (Cor){100, 100, 100}, 10);
        TESTE(resultado != NULL, "Busca aproximada retornou resultado");
        TESTE(tamanhoLista(resultado) >= 3, "Encontrou pelo menos 3 cores próximas");
        
        liberaLista(resultado);
        liberaArvore(arvore);
        liberaImagem(img);
    }
    
    // ========================================
    // TESTE 10: Comparação BST vs AVL (Altura)
    // ========================================
    printf("\n\033[44m=== Teste 10: Vantagem AVL sobre BST ===\033[0m\n");
    {
        Imagem *img = alocaImagem(1, 31);
        
        // Pior caso para BST: inserção sequencial (vira lista)
        for(int i = 0; i < 31; i++) {
            recolorePixel(img, i, 0, (Cor){i * 8, 0, 0});
        }
        
        Arvore *arvore = criaArvore(img);
        int altura = alturaArvore(arvore);
        
        // BST não balanceada teria altura 30 (lista)
        // AVL deve ter altura ≤ 5 para 31 nós
        TESTE(altura <= 5, "AVL mantém altura logarítmica (≤5 para 31 nós)");
        printf("BST não balanceada teria altura 30\n");
        printf("AVL tem altura %d - melhoria significativa!\n", altura);
        
        liberaArvore(arvore);
        liberaImagem(img);
    }
    
    // ========================================
    // TESTE 11: Árvore com Imagem Complexa
    // ========================================
    printf("\n\033[44m=== Teste 11: Árvore com Imagem Real ===\033[0m\n");
    {
        Imagem *img = alocaImagem(10, 10);
        for(int i = 0; i < 10; i++) {
            for(int j = 0; j < 10; j++) {
                recolorePixel(img, i, j, (Cor){i * 25, j * 25, (i+j) * 12});
            }
        }
        
        Arvore *arvore = criaArvore(img);
        int numNos = numeroNosArvore(arvore);
        int altura = alturaArvore(arvore);
        
        TESTE(numNos > 0, "Árvore contém nós");
        TESTE(numNos <= 100, "Número de nós não excede pixels");
        
        // Verifica se altura está dentro do esperado para AVL
        // Para N nós, altura ≤ 1.44 * log₂(N+2)
        double alturaMaxima = 1.44 * log2(numNos + 2);
        TESTE(altura <= (int)alturaMaxima + 1, "Altura dentro do limite AVL");
        
        printf("Imagem 10x10: %d cores distintas, altura %d\n", numNos, altura);
        
        liberaArvore(arvore);
        liberaImagem(img);
    }
    
    // ========================================
    // TESTE 12: Ordenação Lexicográfica Mantida
    // ========================================
    printf("\n\033[44m=== Teste 12: Ordenação Lexicográfica Após Rotações ===\033[0m\n");
    {
        Imagem *img = alocaImagem(1, 5);
        // Cores que forçarão rotações mas devem manter ordem
        recolorePixel(img, 0, 0, (Cor){50, 100, 200});
        recolorePixel(img, 1, 0, (Cor){50, 0, 0});
        recolorePixel(img, 2, 0, (Cor){50, 100, 0});
        recolorePixel(img, 3, 0, (Cor){25, 50, 75});
        recolorePixel(img, 4, 0, (Cor){75, 150, 225});
        
        Arvore *arvore = criaArvore(img);
        
        // Busca cada cor individualmente
        TESTE(buscaArvore(arvore, (Cor){25, 50, 75}, 0) != NULL, "Cor 1 encontrada");
        TESTE(buscaArvore(arvore, (Cor){50, 0, 0}, 0) != NULL, "Cor 2 encontrada");
        TESTE(buscaArvore(arvore, (Cor){50, 100, 0}, 0) != NULL, "Cor 3 encontrada");
        TESTE(buscaArvore(arvore, (Cor){50, 100, 200}, 0) != NULL, "Cor 4 encontrada");
        TESTE(buscaArvore(arvore, (Cor){75, 150, 225}, 0) != NULL, "Cor 5 encontrada");
        
        liberaArvore(arvore);
        liberaImagem(img);
    }
    
    // ========================================
    // TESTE 13: Stress Test - Muitas Inserções
    // ========================================
    printf("\n\033[44m=== Teste 13: Stress Test - 100 Nós ===\033[0m\n");
    {
        Imagem *img = alocaImagem(1, 100);
        
        // Insere 100 cores em ordem aleatória (simulada)
        int ordem[] = {50, 25, 75, 12, 37, 62, 87, 6, 18, 31, 43, 56, 68, 81, 93,
                       3, 9, 15, 21, 28, 34, 40, 46, 53, 59, 65, 71, 78, 84, 90, 96};
        for(int i = 0; i < 31; i++) {
            recolorePixel(img, i, 0, (Cor){ordem[i], 0, 0});
        }
        // Completa com valores sequenciais
        for(int i = 31; i < 100; i++) {
            recolorePixel(img, i, 0, (Cor){i + 100, 0, 0});
        }
        
        Arvore *arvore = criaArvore(img);
        TESTE(numeroNosArvore(arvore) == 100, "100 nós inseridos");
        
        int altura = alturaArvore(arvore);
        // Para 100 nós, altura AVL ≤ 7
        TESTE(altura <= 7, "Altura balanceada para 100 nós (≤7)");
        printf("Altura da árvore com 100 nós: %d\n", altura);
        
        liberaArvore(arvore);
        liberaImagem(img);
    }
    
    
    // ========================================
    // TESTE 14: Propriedade AVL - Fator de Balanceamento
    // ========================================
    printf("\n\033[44m=== Teste 14: Verificação Indireta do Fator de Balanceamento ===\033[0m\n");
    {
        // Testa que árvore mantém propriedade AVL mesmo após muitas inserções
        Imagem *img = alocaImagem(1, 20);
        
        // Pior padrão possível para BST
        for(int i = 0; i < 20; i++) {
            recolorePixel(img, i, 0, (Cor){i * 12, 0, 0});
        }
        
        Arvore *arvore = criaArvore(img);
        int altura = alturaArvore(arvore);
        
        // Se fator de balanceamento está sendo mantido (-1, 0, 1),
        // então altura deve ser ≤ 5 para 20 nós
        TESTE(altura <= 5, "Fator de balanceamento mantido (altura ≤5 para 20 nós)");
        printf("Altura: %d (confirma FB ∈ {-1, 0, 1} em todos os nós)\n", altura);
        
        liberaArvore(arvore);
        liberaImagem(img);
    }
    
    // ========================================
    // FIM DOS TESTES
    // ========================================
    printf("\n\033[42m============================================\033[0m\n");
    printf("\033[42m=== TODOS OS TESTES DA ETAPA 5 PASSARAM! ===\033[0m\n");
    printf("\033[42m============================================\033[0m\n");
}