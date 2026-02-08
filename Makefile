# ==========================================================
# Makefile - FotoXop
# =========================================================

# 1️⃣ Variáveis (para facilitar manutenção)
CFLAGS=-O0 -std=c11 -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -fsanitize=address,undefined,leak 
CLIBS=-lm
LDFLAGS = -fsanitize=address,undefined,leak # Flags usadas no linking (geram o executável)
PKG = `pkg-config --cflags --libs gtk+-3.0`
EXECUTAVEL=FotoXop.exe # nome do executável final

# ==========================================================
# 2️⃣ Alvo principal (primeiro alvo → padrão ao rodar "make")
# ==========================================================

all: $(EXECUTAVEL)

$(EXECUTAVEL): main.o Cor.o Filtros.o Imagem.o AVL.o Lista.o Pilha.o UI.o Testes.o Util.o
	@echo "🔧 Ligando objetos e gerando executável..."
	@gcc main.o Cor.o Filtros.o Imagem.o AVL.o Lista.o Pilha.o UI.o Testes.o Util.o $(CLIBS) $(LDFLAGS) $(PKG) -o $(EXECUTAVEL)
	@echo "✅ Compilação concluída."
	@echo "🚀 Para executar o programa digite ./$(EXECUTAVEL) no terminal."

# ==========================================================
# 3️⃣ Regras para gerar os arquivos objeto
#===========================================================

main.o: main.c
	@echo "🧩 Compilando main.c"
	@gcc $(CFLAGS) $(PKG) -c main.c

Filtros.o: Filtros.c
	@echo "🧩 Compilando Filtros.c"
	@gcc $(CFLAGS) $(PKG) -c Filtros.c

Imagem.o: Imagem.c
	@echo "🧩 Compilando Imagem.c"
	@gcc $(CFLAGS) $(PKG) -c Imagem.c

AVL.o: AVL.c
	@echo "🧩 Compilando AVL.c"
	@gcc $(CFLAGS) $(PKG) -c AVL.c

Lista.o: Lista.c
	@echo "🧩 Compilando Lista.c"
	@gcc $(CFLAGS) $(PKG) -c Lista.c

Pilha.o: Pilha.c
	@echo "🧩 Compilando Pilha.c"
	@gcc $(CFLAGS) $(PKG) -c Pilha.c

UI.o: UI.c
	@echo "🧩 Compilando UI.c"
	@gcc $(CFLAGS) $(PKG) -c UI.c

Cor.o: Cor.c
	@echo "🧩 Compilando Cor.c"
	@gcc $(CFLAGS) -c Cor.c

Testes.o: Testes.c
	@echo "🧩 Compilando Testes.c"
	@gcc $(CFLAGS) $(PKG) -c Testes.c
	
Util.o: Util.c
	@echo "🧩 Compilando Util.c"
	@gcc $(CFLAGS) -c Util.c




# ==========================================================
# 4️⃣ Alvos auxiliares (não geram arquivos → PHONY)
# ==========================================================

.PHONY: clean run

# Limpa os arquivos temporários
clean: 
	@echo "🧹 Limpando arquivos compilados..."
	@rm -rf *.o $(EXECUTAVEL)
	@echo "🧼 Diretório limpo!"
	
# Executa o programa (compila antes se necessário)
run: $(EXECUTAVEL)
	@echo "🚀 Executando o programa"
	@ASAN_OPTIONS=detect_leaks=1 LSAN_OPTIONS=suppressions=gtk.supp:print_suppressions=0 ./$(EXECUTAVEL)
# 	@LSAN_OPTIONS=suppressions=gtk.supp ./$(EXECUTAVEL)

