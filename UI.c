#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "AVL.h"
#include "UI.h"
#include "Imagem.h"
#include "Filtros.h"
#include "Pilha.h"
#include "Cor.h"
#include "Util.h"


// Variáveis globais
GtkWidget *janelaPrincipal;
GtkWidget *widgetImagem;
GdkPixbuf *pixbufAtual = NULL;
GdkPixbuf *pixbufPreVisualizacao = NULL;
Imagem *imagemAtual = NULL;
Imagem *imagemPreVisualizacao = NULL;

Imagem *copiaDaImagem = NULL;
Imagem *imagemOriginal = NULL;

#if ETAPA >= 3
Pilha *pilhaDesfazer = NULL;
#endif

#if ETAPA >= 6
Arvore *arvoreCores = NULL;
#endif

/* coordenadas do clique */
int cliqueX = 0, cliqueY = 0;

/* cores */
GdkRGBA corAlvo;
GdkRGBA corNova;

/* widgets da janela Trocar Cor */
GtkWidget *janelaTrocaCor;
GtkWidget *spinR, *spinG, *spinB, *spinTolerancia;
GtkWidget *areaCorAtual, *areaCorNova;

/* widgets da janela Brilho e Contraste */
GtkWidget *janelaBrilhoContraste = NULL;
GtkWidget *escalaBrilho = NULL;
GtkWidget *escalaContraste = NULL;

/* Menus */
GtkWidget *itemFiltros;
GtkWidget *itemEditarDesfazer;
GtkWidget *itemArquivoSalvar;
GtkWidget *itemEditarImagemOriginal;

void atualizaDisplay() {
    if (!imagemAtual) {
        ERRO("Nenhuma imagem para exibir");
        return;
    }

#if ETAPA >= 3
    // Ativa/desativa a opção de desfazer
    gtk_widget_set_sensitive(itemEditarDesfazer, !pilhaVazia(pilhaDesfazer));
#endif

    // Liberar pixbuf anterior se existir
    if (pixbufAtual) {
        g_object_unref(pixbufAtual);
        pixbufAtual = NULL;
    }

    // Criar novo pixbuf
    pixbufAtual = imagemParaPixbuf(imagemAtual);

#if ETAPA >= 6
    // Atualiza a árvore de cores
    liberaArvore(arvoreCores);
    arvoreCores = criaArvore(imagemAtual);
#endif    

    if (pixbufAtual) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(widgetImagem), pixbufAtual);
    }
    else {
        ERRO("Erro ao criar pixbuf");
    }
}

void aplicarBrilho(int delta) {
    if (!imagemAtual)
        return;

    for (int i = 0; i < obtemAltura(imagemAtual); i++) {
        for (int j = 0; j < obtemLargura(imagemAtual); j++) {
            Cor pixel = obtemCorPixel(imagemAtual, i, j);
            Byte r = ajustaCor(pixel.r + delta);
            Byte g = ajustaCor(pixel.g + delta);
            Byte b = ajustaCor(pixel.b + delta);

            Cor novaCor = {r, g, b};
            recolorePixel(imagemAtual, i, j, novaCor);
        }
    }

    atualizaDisplay();
}

// Callbacks dos menus
void abrirImagemCallback(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Abrir Imagem",
        GTK_WINDOW(janelaPrincipal),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Cancelar", GTK_RESPONSE_CANCEL,
        "_Abrir", GTK_RESPONSE_ACCEPT,
        NULL);

    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Imagens PPM/PNM");
    gtk_file_filter_add_pattern(filter, "*.ppm");
    gtk_file_filter_add_pattern(filter, "*.pnm");
    gtk_file_filter_add_pattern(filter, "*.PPM");
    gtk_file_filter_add_pattern(filter, "*.PNM");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    // Adicionar filtro "Todos os arquivos"
    GtkFileFilter *filterTodos = gtk_file_filter_new();
    gtk_file_filter_set_name(filterTodos, "Todos os arquivos");
    gtk_file_filter_add_pattern(filterTodos, "*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filterTodos);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        g_print("\n=== Carregando arquivo: %s ===\n", filename);

        // Liberar imagem anterior
        if (imagemAtual) {
            liberaImagem(imagemAtual);
            imagemAtual = NULL;
        }
        if (imagemOriginal) {
            liberaImagem(imagemOriginal); 
            imagemOriginal = NULL;
        }
        
        // Liberar pixbufs anteriores
        if (pixbufAtual) {
            g_object_unref(pixbufAtual);
            pixbufAtual = NULL;
        }
        if (pixbufPreVisualizacao) {
            g_object_unref(pixbufPreVisualizacao);
            pixbufPreVisualizacao = NULL;
        }
#if ETAPA >= 3
        // Liberar pilha de desfazer anterior
        if (pilhaDesfazer) {
            liberaPilha(pilhaDesfazer);
            pilhaDesfazer = NULL;
        }
#endif
#if ETAPA >= 6
        // Liberar árvore de cores anterior
        if (arvoreCores) {
            liberaArvore(arvoreCores);
            arvoreCores = NULL;
        }
#endif
        // Carregar nova imagem
        imagemAtual = carregaImagem(filename);
        
        if (imagemAtual) {
            g_print("Imagem carregada: %dx%d\n", obtemAltura(imagemAtual), obtemLargura(imagemAtual));
            imagemOriginal = copiaImagem(imagemAtual);
            gtk_widget_set_sensitive(itemArquivoSalvar, true);
#if ETAPA >= 1
            gtk_widget_set_sensitive(itemFiltros, true);
#endif
#if ETAPA >= 3
            gtk_widget_set_sensitive(itemEditarImagemOriginal, true);
            // Cria a Pilha de Desfazer
            pilhaDesfazer = criaPilha();
#endif
            // Atualiza display
            atualizaDisplay();
        }
        else {
            g_print("ERRO: Falha ao carregar imagem!\n");

            // Mostrar mensagem de erro
            GtkWidget *dialogErro = gtk_message_dialog_new(
                GTK_WINDOW(janelaPrincipal),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Erro ao carregar imagem: %s", filename);
            gtk_dialog_run(GTK_DIALOG(dialogErro));
            gtk_widget_destroy(dialogErro);
        }

        g_free(filename);
    }
    gtk_widget_destroy(dialog);
}

void salvarImagemCallback(GtkWidget *widget, gpointer data) {
    if (!imagemAtual) {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(janelaPrincipal),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Nenhuma imagem carregada!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Salvar Imagem",
        GTK_WINDOW(janelaPrincipal),
        GTK_FILE_CHOOSER_ACTION_SAVE,
        "_Cancelar", GTK_RESPONSE_CANCEL,
        "_Salvar", GTK_RESPONSE_ACCEPT,
        NULL);

    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "imagem.ppm");

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        salvaImagem(imagemAtual, filename);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

void sairCallback(GtkWidget *widget, gpointer data) {
    // Libera recursos
    if (imagemAtual) {
        liberaImagem(imagemAtual);
        imagemAtual = NULL;
    }
    if (copiaDaImagem) {
        liberaImagem(copiaDaImagem);
        copiaDaImagem = NULL;
    }
    if (imagemOriginal) {
        liberaImagem(imagemOriginal);
        imagemOriginal = NULL;
    }
#if ETAPA >= 3
    if(pilhaDesfazer) {
        liberaPilha(pilhaDesfazer);
        pilhaDesfazer = NULL;
    }
#endif
#if ETAPA >= 6
    if(arvoreCores) {
        liberaArvore(arvoreCores);
        arvoreCores = NULL;
    }
#endif
    if (pixbufAtual)
        g_clear_object(&pixbufAtual);
    if (pixbufPreVisualizacao)
        g_clear_object(&pixbufPreVisualizacao);

    gtk_main_quit();
}

void desfazerCallback(GtkWidget *widget, gpointer data) {
#if ETAPA >= 3
    AVISO("UI.c: Não inclui uma cópia da imagem atual na pilha de desfazer na função 'desfazerCallback'");
    if (pilhaDesfazer && !pilhaVazia(pilhaDesfazer)) {
        // Com você :)
        
        atualizaDisplay();
    }
    
#endif
}

void filtroEscalaCinzaCallback(GtkWidget *widget, gpointer data) {
    if (!imagemAtual)
        return;
#if ETAPA >= 3
    AVISO("UI.c: Não inclui uma cópia da imagem atual na pilha de desfazer na função 'filtroEscalaCinzaCallback'");
    // Com você :)

#endif
    escalaDeCinzaImagem(imagemAtual);
    atualizaDisplay();
}

void deteccaoBordasLaplaceCallback(GtkWidget *widget, gpointer data){
    if (!imagemAtual)
        return;
#if ETAPA >= 3
    AVISO("UI.c: Não inclui uma cópia da imagem atual na pilha de desfazer na função 'deteccaoBordasLaplaceCallback'");
    // Com você :)

#endif
    deteccaoBordasLaplace(imagemAtual);
    atualizaDisplay();
}

void filtroSobelCallback(GtkWidget *widget, gpointer data){
    if (!imagemAtual)
        return;
#if ETAPA >= 3
    AVISO("UI.c: Não inclui uma cópia da imagem atual na pilha de desfazer na função 'filtroSobelCallback'");
    // Com você :)

#endif
    filtroSobel(imagemAtual);
    atualizaDisplay();
}

void meuFiltroCallback(GtkWidget *widget, gpointer data){
    if (!imagemAtual)
        return;
#if ETAPA >= 3
    AVISO("UI.c: Não inclui uma cópia da imagem atual na pilha de desfazer na função 'meuFiltroCallback'");
    // Com você :)

#endif
    meuFiltro(imagemAtual);
    atualizaDisplay();
}

void imagemOriginalCallback(GtkWidget *widget, gpointer data) {
    if (!imagemOriginal)
        return;
#if ETAPA >= 3
    AVISO("UI.c: Não inclui uma cópia da imagem atual na pilha de desfazer na função 'imagemOriginalCallback'");
    // Com você :)

#endif
    // Libera a imagem atual antes de copiar
    if (imagemAtual) {
        liberaImagem(imagemAtual);
    }
    
    imagemAtual = copiaImagem(imagemOriginal);
    atualizaDisplay();
}

/* Aplica brilho + contraste (pré-visualização) */
void previewBrilhoContraste(GtkRange *range, gpointer data) {
    if (!pixbufAtual)
        return;

    int brilho = gtk_range_get_value(GTK_RANGE(escalaBrilho));
    double contraste = gtk_range_get_value(GTK_RANGE(escalaContraste));

    double c = (contraste + 100.0) / 100.0;

    if(imagemPreVisualizacao)
        liberaImagem(imagemPreVisualizacao);
    imagemPreVisualizacao = copiaImagem(imagemAtual);

    for(int i = 0; i < obtemAltura(imagemPreVisualizacao); i++) {
        for(int j = 0; j < obtemLargura(imagemPreVisualizacao); j++) {
            Cor pixel = obtemCorPixel(imagemPreVisualizacao, i, j);

            int r = ((pixel.r - 128) * c) + 128 + brilho;
            int g = ((pixel.g - 128) * c) + 128 + brilho;
            int b = ((pixel.b - 128) * c) + 128 + brilho;

            Cor novaCor = {ajustaCor(r), ajustaCor(g), ajustaCor(b)};
            recolorePixel(imagemPreVisualizacao, i, j, novaCor);
        }
    }
    if (pixbufPreVisualizacao)
        g_clear_object(&pixbufPreVisualizacao);
    pixbufPreVisualizacao = imagemParaPixbuf(imagemPreVisualizacao);
    gtk_image_set_from_pixbuf(GTK_IMAGE(widgetImagem), pixbufPreVisualizacao);
}

void aplicarAlteracoes() {
    if(!imagemPreVisualizacao)
        return;
#if ETAPA >= 3
    AVISO("UI.c: Não inclui uma cópia da imagem atual na pilha de desfazer na função 'aplicarAlteracoes'");
    // Com você :)

#endif
    liberaImagem(imagemAtual);
    imagemAtual = copiaImagem(imagemPreVisualizacao);
    if(imagemAtual == NULL) {
        ERRO("Erro ao aplicar alterações na imagem.");
        return;
    }
    liberaImagem(imagemPreVisualizacao);
    imagemPreVisualizacao = NULL;
    atualizaDisplay();
}

void botaoAplicarTrocaCorClicked(GtkWidget *b, gpointer data) {
    aplicarAlteracoes();
    gtk_widget_destroy(janelaTrocaCor);
    janelaTrocaCor = NULL;
}

void botaoCancelarTrocaCorClicked(GtkWidget *b, gpointer data) {
    gtk_image_set_from_pixbuf(GTK_IMAGE(widgetImagem), pixbufAtual);
    gtk_widget_destroy(janelaTrocaCor);
    janelaTrocaCor = NULL;
}

void botaoAplicarBrilhoClicked(GtkWidget *b, gpointer data) {
    aplicarAlteracoes();
    gtk_widget_destroy(janelaBrilhoContraste);
    janelaBrilhoContraste = NULL;
}

void botaoCancelarBrilhoClicked(GtkWidget *b, gpointer data) {
    gtk_image_set_from_pixbuf(GTK_IMAGE(widgetImagem), pixbufAtual);
    gtk_widget_destroy(janelaBrilhoContraste);
    janelaBrilhoContraste = NULL;
}

/* Função que cria e exibe a janela de brilho */
void abrirJanelaBrilhoContraste(GtkWidget *widget, gpointer data) {
    if (janelaBrilhoContraste != NULL) {
        gtk_window_present(GTK_WINDOW(janelaBrilhoContraste));
        return;
    }

    janelaBrilhoContraste = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(janelaBrilhoContraste), "Brilho e Contraste");
    gtk_window_set_resizable(GTK_WINDOW(janelaBrilhoContraste), FALSE);
    gtk_window_set_type_hint(GTK_WINDOW(janelaBrilhoContraste), GDK_WINDOW_TYPE_HINT_UTILITY);

    g_signal_connect(janelaBrilhoContraste, "destroy", G_CALLBACK(botaoCancelarBrilhoClicked), &janelaBrilhoContraste);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_set_border_width(GTK_CONTAINER(box), 12);
    gtk_container_add(GTK_CONTAINER(janelaBrilhoContraste), box);

    /* Brilho */
    GtkWidget *lblBrilho = gtk_label_new("Brilho");
    escalaBrilho = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -127, 127, 1);
    gtk_range_set_value(GTK_RANGE(escalaBrilho), 0);

    /* Contraste */
    GtkWidget *lblContraste = gtk_label_new("Contraste");
    escalaContraste = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -100, 100, 1);
    gtk_range_set_value(GTK_RANGE(escalaContraste), 0);

    g_signal_connect(escalaBrilho, "value-changed", G_CALLBACK(previewBrilhoContraste), NULL);
    g_signal_connect(escalaContraste, "value-changed", G_CALLBACK(previewBrilhoContraste), NULL);

    /* Botões */
    GtkWidget *boxBotoes = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *btnAplicar = gtk_button_new_with_label("Aplicar");
    GtkWidget *btnCancelar = gtk_button_new_with_label("Cancelar");

    g_signal_connect(btnAplicar, "clicked", G_CALLBACK(botaoAplicarBrilhoClicked), NULL);
    g_signal_connect(btnCancelar, "clicked", G_CALLBACK(botaoCancelarBrilhoClicked), NULL);

    gtk_box_pack_end(GTK_BOX(boxBotoes), btnAplicar, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(boxBotoes), btnCancelar, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(box), lblBrilho, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), escalaBrilho, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), lblContraste, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), escalaContraste, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), boxBotoes, FALSE, FALSE, 0);

    gtk_widget_show_all(janelaBrilhoContraste);
}

bool desenharCor(GtkWidget *w, cairo_t *cr, gpointer data) {
    GdkRGBA *c = (GdkRGBA *)data;
    GtkAllocation a;
    gtk_widget_get_allocation(w, &a);

    cairo_set_source_rgba(cr, c->red, c->green, c->blue, 1.0);
    cairo_rectangle(cr, 0, 0, a.width, a.height);
    cairo_fill(cr);
    return FALSE;
}

void capturaCorPixel() {
    int w = gdk_pixbuf_get_width(pixbufAtual);
    int h = gdk_pixbuf_get_height(pixbufAtual);

    if (cliqueX < 0 || cliqueY < 0 || cliqueX >= w || cliqueY >= h)
        return;

    int rs = gdk_pixbuf_get_rowstride(pixbufAtual);
    int nc = gdk_pixbuf_get_n_channels(pixbufAtual);
    guchar *p = gdk_pixbuf_get_pixels(pixbufAtual);

    guchar *px = p + cliqueY * rs + cliqueX * nc;

    corAlvo.red = px[0] / 255.0;
    corAlvo.green = px[1] / 255.0;
    corAlvo.blue = px[2] / 255.0;
    corAlvo.alpha = 1.0;

    corNova = corAlvo;
}

void previewTrocaCor(GtkSpinButton *sb, gpointer data) {
    corNova.red = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinR)) / 255.0;
    corNova.green = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinG)) / 255.0;
    corNova.blue = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinB)) / 255.0;
    corNova.alpha = 1.0;

    gtk_widget_queue_draw(areaCorNova);

    int w = gdk_pixbuf_get_width(pixbufAtual);
    int h = gdk_pixbuf_get_height(pixbufAtual);
    int rs = gdk_pixbuf_get_rowstride(pixbufAtual);
    int nc = gdk_pixbuf_get_n_channels(pixbufAtual);

    int tolerancia = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinTolerancia));

    if(imagemPreVisualizacao)
        liberaImagem(imagemPreVisualizacao);
    imagemPreVisualizacao = copiaImagem(imagemAtual);

    // Cor do pixel alvo convertida para 0-255
    int r0 = corAlvo.red * 255;
    int g0 = corAlvo.green * 255;
    int b0 = corAlvo.blue * 255;

    // Nova cor do pixel convertida para 0-255
    int rn = corNova.red * 255;
    int gn = corNova.green * 255;
    int bn = corNova.blue * 255;

#if ETAPA >= 6
    AVISO("UI.c: Ainda não gerei a lista de posições que devem ser alteradas na função 'previewTrocaCor'");
    // Lembre-se que se a tolerância for zero, você não deve liberar a lista, pois ela é interna à árvore.
    // Caso contário, se a tolerância for maior que zero, você deve liberar a lista ao final.

    // Com você :)
    
#endif
    if (pixbufPreVisualizacao)
        g_clear_object(&pixbufPreVisualizacao);
    pixbufPreVisualizacao = imagemParaPixbuf(imagemPreVisualizacao);
    gtk_image_set_from_pixbuf(GTK_IMAGE(widgetImagem), pixbufPreVisualizacao);
}

void abrirJanelaTrocaCor(GtkWidget *w, gpointer d) {
    janelaTrocaCor = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(janelaTrocaCor), "Trocar Cor");
    gtk_window_set_default_size(GTK_WINDOW(janelaTrocaCor), 250, 320);
    gtk_window_set_modal(GTK_WINDOW(janelaTrocaCor), TRUE);

    g_signal_connect(janelaTrocaCor, "destroy", G_CALLBACK(botaoCancelarTrocaCorClicked), &janelaBrilhoContraste);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(janelaTrocaCor), vbox);

    areaCorAtual = gtk_drawing_area_new();
    areaCorNova = gtk_drawing_area_new();
    gtk_widget_set_size_request(areaCorAtual, 80, 50);
    gtk_widget_set_size_request(areaCorNova, 80, 50);

    g_signal_connect(areaCorAtual, "draw", G_CALLBACK(desenharCor), &corAlvo);
    g_signal_connect(areaCorNova, "draw", G_CALLBACK(desenharCor), &corNova);

    GtkWidget *lblAtual = gtk_label_new("Cor atual");
    GtkWidget *lblNova = gtk_label_new("Nova cor");

    GtkWidget *c1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    GtkWidget *c2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);

    gtk_box_pack_start(GTK_BOX(c1), lblAtual, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(c1), areaCorAtual, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(c2), lblNova, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(c2), areaCorNova, FALSE, FALSE, 0);

    GtkWidget *hc = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_box_pack_start(GTK_BOX(hc), c1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hc), c2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hc, FALSE, FALSE, 0);

    spinR = gtk_spin_button_new_with_range(0, 255, 1);
    spinG = gtk_spin_button_new_with_range(0, 255, 1);
    spinB = gtk_spin_button_new_with_range(0, 255, 1);
    spinTolerancia = gtk_spin_button_new_with_range(0, 450, 1);

    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spinR), TRUE);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spinG), TRUE);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spinB), TRUE);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spinTolerancia), TRUE);

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinR), corAlvo.red * 255);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinG), corAlvo.green * 255);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinB), corAlvo.blue * 255);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinTolerancia), 0);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 6);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("R:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), spinR, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("G:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), spinG, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("B:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), spinB, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Tolerância:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), spinTolerancia, 1, 3, 1, 1);

    gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 0);

    g_signal_connect(spinR, "value-changed", G_CALLBACK(previewTrocaCor), NULL);
    g_signal_connect(spinG, "value-changed", G_CALLBACK(previewTrocaCor), NULL);
    g_signal_connect(spinB, "value-changed", G_CALLBACK(previewTrocaCor), NULL);
    g_signal_connect(spinTolerancia, "value-changed", G_CALLBACK(previewTrocaCor), NULL);

    GtkWidget *hb = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *btnAplicar = gtk_button_new_with_label("Aplicar");
    GtkWidget *btnCancelar = gtk_button_new_with_label("Cancelar");

    g_signal_connect(btnAplicar, "clicked", G_CALLBACK(botaoAplicarTrocaCorClicked), NULL);
    g_signal_connect(btnCancelar, "clicked", G_CALLBACK(botaoCancelarTrocaCorClicked), NULL);

    gtk_box_pack_end(GTK_BOX(hb), btnAplicar, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(hb), btnCancelar, FALSE, FALSE, 0);

    gtk_box_pack_end(GTK_BOX(vbox), hb, FALSE, FALSE, 0);

    gtk_widget_show_all(janelaTrocaCor);
}

void menuTrocarCor(GtkWidget *w, gpointer d) {
    capturaCorPixel();
    abrirJanelaTrocaCor(w, d);
}

bool onClick(GtkWidget *w, GdkEventButton *e, gpointer d) {
    if (e->type == GDK_BUTTON_PRESS && e->button == 3) {
        if (!pixbufAtual)
            return true;

        // 1. Obtém as dimensões do Widget (Janela/Área visível)
        int widgetW = gtk_widget_get_allocated_width(w);
        int widgetH = gtk_widget_get_allocated_height(w);

        // 2. Obtém as dimensões reais da Imagem
        int imgW = gdk_pixbuf_get_width(pixbufAtual);
        int imgH = gdk_pixbuf_get_height(pixbufAtual);

        // 3. Calcula o offset (centralização)
        // Se o widget for maior que a imagem, a imagem está centralizada.
        // O offset é a metade da sobra.
        int offsetX = (widgetW > imgW) ? (widgetW - imgW) / 2 : 0;
        int offsetY = (widgetH > imgH) ? (widgetH - imgH) / 2 : 0;

        // 4. Calcula a posição real no pixel
        int tempX = (int)e->x - offsetX;
        int tempY = (int)e->y - offsetY;

        // 5. Verifica se o clique foi DENTRO da imagem
        // Se clicar na borda cinza, ignoramos.
        if (tempX < 0 || tempY < 0 || tempX >= imgW || tempY >= imgH) {
            g_print("Clique fora da área da imagem.\n");
            return false;
        }

        // Se passou, atualiza as variáveis globais
        cliqueX = tempX;
        cliqueY = tempY;

        g_print("Clique detectado em: %d, %d\n", cliqueX, cliqueY);

        GtkWidget *menu = gtk_menu_new();
        GtkWidget *item = gtk_menu_item_new_with_label("Trocar cor");

        g_signal_connect(item, "activate", G_CALLBACK(menuTrocarCor), NULL);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

        gtk_widget_show_all(menu);
        gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent *)e);
        return true;
    }
    return false;
}

void inicializa(int *argc, char *argv[]) {
    gtk_init(argc, &argv);

    // Carrega arquivo de estilo
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "estilo.css", NULL);
    GdkScreen *screen = gdk_screen_get_default();
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Cria a janela principal
    janelaPrincipal = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(janelaPrincipal), "FotoXop");
    gtk_window_set_default_size(GTK_WINDOW(janelaPrincipal), 1000, 700);
    g_signal_connect(janelaPrincipal, "destroy", G_CALLBACK(sairCallback), NULL);

    // Cria o grupo de aceleradores
    GtkAccelGroup *grupoAceleradores = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(janelaPrincipal), grupoAceleradores);

    // Cria layout vertical
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(janelaPrincipal), vbox);

    // Cria barra de menu
    GtkWidget *menuPrincipal = gtk_menu_bar_new();

    // Menu Arquivo
    GtkWidget *menuArquivo = gtk_menu_new();
    GtkWidget *itemArquivo = gtk_menu_item_new_with_label("Arquivo");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(itemArquivo), menuArquivo);

    GtkWidget *itemArquivoAbrir = gtk_menu_item_new_with_label("📂 Abrir imagem");
    itemArquivoSalvar = gtk_menu_item_new_with_label("💾 Salvar imagem");
    GtkWidget *itemArquivoSair = gtk_menu_item_new_with_label("Sair");
    gtk_widget_set_sensitive(itemArquivoSalvar, false);

    g_signal_connect(itemArquivoAbrir, "activate", G_CALLBACK(abrirImagemCallback), NULL);
    g_signal_connect(itemArquivoSalvar, "activate", G_CALLBACK(salvarImagemCallback), NULL);
    g_signal_connect(itemArquivoSair, "activate", G_CALLBACK(sairCallback), NULL);

    gtk_menu_shell_append(GTK_MENU_SHELL(menuArquivo), itemArquivoAbrir);
    gtk_menu_shell_append(GTK_MENU_SHELL(menuArquivo), itemArquivoSalvar);
    gtk_menu_shell_append(GTK_MENU_SHELL(menuArquivo), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(menuArquivo), itemArquivoSair);

    // Atalhos do menu Arquivo
    gtk_widget_add_accelerator(itemArquivoAbrir, "activate", grupoAceleradores, GDK_KEY_a, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(itemArquivoSalvar, "activate", grupoAceleradores, GDK_KEY_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(itemArquivoSair, "activate", grupoAceleradores, GDK_KEY_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
#if ETAPA >= 3
    // Menu Editar
    GtkWidget *menuEditar = gtk_menu_new();
    GtkWidget *itemEditar = gtk_menu_item_new_with_label("Editar");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(itemEditar), menuEditar);

    itemEditarDesfazer = gtk_menu_item_new_with_label("Desfazer");
    itemEditarImagemOriginal = gtk_menu_item_new_with_label("Imagem Original");
    gtk_widget_set_sensitive(itemEditarDesfazer, false);
    gtk_widget_set_sensitive(itemEditarImagemOriginal, false);

    g_signal_connect(itemEditarDesfazer, "activate", G_CALLBACK(desfazerCallback), NULL);
    g_signal_connect(itemEditarImagemOriginal, "activate", G_CALLBACK(imagemOriginalCallback), NULL);

    gtk_menu_shell_append(GTK_MENU_SHELL(menuEditar), itemEditarDesfazer);
    gtk_menu_shell_append(GTK_MENU_SHELL(menuEditar), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(menuEditar), itemEditarImagemOriginal);

    // Atalhos do menu Editar
    gtk_widget_add_accelerator(itemEditarDesfazer, "activate", grupoAceleradores, GDK_KEY_z, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(itemEditarImagemOriginal, "activate", grupoAceleradores, GDK_KEY_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
#endif
#if ETAPA >= 1
    // Menu Filtros
    GtkWidget *menuFiltros = gtk_menu_new();
    itemFiltros = gtk_menu_item_new_with_label("✨ Filtros");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(itemFiltros), menuFiltros);
    gtk_widget_set_sensitive(itemFiltros, FALSE);

    GtkWidget *itemFiltrosEscalaCinza = gtk_menu_item_new_with_label("Escala de Cinza");
    GtkWidget *itemFiltrosDeteccaoBordas = gtk_menu_item_new_with_label("Detecção de Bordas de Laplace");
    GtkWidget *itemFiltrosSobel = gtk_menu_item_new_with_label("Filtro de Sobel");
    GtkWidget *itemFiltrosBrilho = gtk_menu_item_new_with_label("Brilho e Contraste");
    GtkWidget *itemFiltrosMeuFiltro = gtk_menu_item_new_with_label("Meu Filtro");

    g_signal_connect(itemFiltrosEscalaCinza, "activate", G_CALLBACK(filtroEscalaCinzaCallback), NULL);
    g_signal_connect(itemFiltrosDeteccaoBordas, "activate", G_CALLBACK(deteccaoBordasLaplaceCallback), NULL);
    g_signal_connect(itemFiltrosSobel, "activate", G_CALLBACK(filtroSobelCallback), NULL);
    g_signal_connect(itemFiltrosMeuFiltro, "activate", G_CALLBACK(meuFiltroCallback), NULL);
    g_signal_connect(itemFiltrosBrilho, "activate", G_CALLBACK(abrirJanelaBrilhoContraste), NULL);

    gtk_menu_shell_append(GTK_MENU_SHELL(menuFiltros), itemFiltrosEscalaCinza);
    gtk_menu_shell_append(GTK_MENU_SHELL(menuFiltros), itemFiltrosBrilho);
    gtk_menu_shell_append(GTK_MENU_SHELL(menuFiltros), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(menuFiltros), itemFiltrosDeteccaoBordas);
    gtk_menu_shell_append(GTK_MENU_SHELL(menuFiltros), itemFiltrosSobel);
    gtk_menu_shell_append(GTK_MENU_SHELL(menuFiltros), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(menuFiltros), itemFiltrosMeuFiltro);

    // Atalhos do menu Filtros
    gtk_widget_add_accelerator(itemFiltrosEscalaCinza, "activate", grupoAceleradores, GDK_KEY_1, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(itemFiltrosBrilho, "activate", grupoAceleradores, GDK_KEY_2, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(itemFiltrosDeteccaoBordas, "activate", grupoAceleradores, GDK_KEY_3, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(itemFiltrosSobel, "activate", grupoAceleradores, GDK_KEY_4, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(itemFiltrosMeuFiltro, "activate", grupoAceleradores, GDK_KEY_5, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
#endif
    // Adiciona menus à barra
    gtk_menu_shell_append(GTK_MENU_SHELL(menuPrincipal), itemArquivo);
#if ETAPA >= 3
    gtk_menu_shell_append(GTK_MENU_SHELL(menuPrincipal), itemEditar);
#endif
#if ETAPA >= 1
    gtk_menu_shell_append(GTK_MENU_SHELL(menuPrincipal), itemFiltros);
#endif
    gtk_box_pack_start(GTK_BOX(vbox), menuPrincipal, FALSE, FALSE, 0);

    // Área de rolagem para a imagem
    GtkWidget *janelaRolagem = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(janelaRolagem), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), janelaRolagem, TRUE, TRUE, 0);

    // Widget de imagem
    widgetImagem = gtk_image_new();
    gtk_widget_set_name(widgetImagem, "image-area");

    GtkWidget *caixaEvento = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(caixaEvento), widgetImagem);
    gtk_container_add(GTK_CONTAINER(janelaRolagem), caixaEvento);
    gtk_widget_add_events(caixaEvento, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(caixaEvento, "button-press-event", G_CALLBACK(onClick), NULL);

    gtk_widget_show_all(janelaPrincipal);
    gtk_main();

}