### Compilador
CC        := gcc

### Flags de Compilacao
LIBS      := -lm  # Bibliotecas usadas
# Flags para compilar
CFLAGS    := $(LIBS)
# Flags para compilar um programa otimizado
FFLAGS    := $(LIBS) -O3 -march=native
# Flags para compilar para debug
DFLAGS    := $(LIBS) -g -Wall -Wextra -pedantic -Werror=implicit-function-declaration -fsanitize=address
# Flags para compilar para o gprof
GFLAGS    := $(LIBS) -pg $(FFLAGS)
# Flags que serao usadas para compilar
FLAGS     := $(FFLAGS)

# Outras Flags
RMFLAGS   := -f -v

### Pasta dos .o
BIN       := bin

### Arquivos
MODULOS   := $(wildcard modulos/*.c)
MODULOS   := $(MODULOS:%.c=$(BIN)/%.o)

MAPA      := $(wildcard mapa/*.c)
MAPA      := $(MAPA:%.c=$(BIN)/%.o)

MINMAX    := $(wildcard minmax/*.c)
MINMAX    := $(MINMAX:%.c=$(BIN)/%.o)

EVOLUTIVO := $(wildcard evolutivo/*.c)
EVOLUTIVO := $(EVOLUTIVO:%.c=$(BIN)/%.o)

# Util
.PHONY: subdirs all clean run_jogo run_mapa run_minmax run_evolutivo run_all

subdirs:
	mkdir -p $(BIN)/mapa
	mkdir -p $(BIN)/minmax
	mkdir -p $(BIN)/modulos
	mkdir -p $(BIN)/evolutivo

### Acoes
all: mapa.out minmax.out evolutivo.out

run_all: run_mapa run_minmax run_evolutivo run_jogo


clean:
	rm $(BIN)/mapa/*.o $(RMFLAGS)
	rm $(BIN)/minmax/*.o $(RMFLAGS)
	rm $(BIN)/modulos/*.o $(RMFLAGS)
	rm $(BIN)/evolutivo/*.o $(RMFLAGS)
	rm *.out $(RMFLAGS)

# Rodar Programa
run_jogo:
	cd jogo && ../venv/bin/python jogo.py

run_mapa: mapa.out
	./mapa.out

run_minmax: minmax.out
	./minmax.out

run_evolutivo: evolutivo.out
	./evolutivo.out

# Compilar programa
mapa.out: subdirs $(MODULOS) $(MAPA)
	$(CC) -o mapa.out $(MODULOS) $(MAPA) $(FLAGS)

minmax.out: subdirs $(MODULOS) $(MINMAX)
	$(CC) -o minmax.out $(MODULOS) $(MINMAX) $(FLAGS)

evolutivo.out: subdirs $(MODULOS) $(EVOLUTIVO)
	$(CC) -o evolutivo.out $(MODULOS) $(EVOLUTIVO) $(FLAGS)

### Compilando ...
$(BIN)/modulos/%.o: modulos/%.c
	$(CC) -c -o $@ $< $(FLAGS)

$(BIN)/mapa/%.o: mapa/%.c
	$(CC) -c -o $@ $< $(FLAGS) -iquote modulos

$(BIN)/minmax/%.o: minmax/%.c
	$(CC) -c -o $@ $< $(FLAGS) -iquote modulos

$(BIN)/evolutivo/%.o: evolutivo/%.c
	$(CC) -c -o $@ $< $(FLAGS) -iquote modulos
