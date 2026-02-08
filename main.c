
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Util.h"
#include "Testes.h"
#include "UI.h"

int main(int argc, char *argv[]) {

#if ETAPA < 0 || ETAPA > 7
        ERRO(ETAPA inválida);
        exit(EXIT_FAILURE);
#endif
#if ETAPA == 0
        testeEtapa0();
        inicializa(&argc, argv);
#endif
#if ETAPA == 1
        inicializa(&argc, argv);
#endif
#if ETAPA == 2
        testeEtapa2();
#endif
#if ETAPA == 3
        inicializa(&argc, argv);
#endif
#if ETAPA == 4
        testeEtapa4();
#endif
#if ETAPA == 5
        testeEtapa5();
#endif
#if ETAPA == 6
        inicializa(&argc, argv);
#endif
// A ETAPA 7 será utilizada pelo professor para correção do trabalho.
#if ETAPA == 7
        testeEtapa0();
        testeEtapa2();
        testeEtapa4();
        testeEtapa5();
        inicializa(&argc, argv);
#endif
        return 0;
}