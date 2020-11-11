### Compilador
CC        := gcc

### Flags de Compilacao
LIBS      :=   # Bibliotecas usadas
# Flags para compilar
CFLAGS    := $(LIBS) 
# Flags para compilar um programa otimizado
FFLAGS    := $(LIBS) -O3 -march=native -w 
# Flags para compilar um programa para debug
DFLAGS    := $(LIBS) -g -Wall -Wextra -pedantic -Werror=implicit-function-declaration -fsanitize=address 
# Flags que serao usadas para compilar
FLAGS     := $(DFLAGS)

# Outras Flags
RMFLAG    := -f -v

### Pasta dos .o
BIN       := bin

### Arquivos
MODULOS   := $(wildcard modulos/*.c)
MODULOS   := $(MODULOS:%.c=$(BIN)/%.o)

MAPA      := $(wildcard mapa/*.c)
MAPA      := $(MAPA:%.c=$(BIN)/%.o)

MINMAX    := $(wildcard minmax/*.c)
MINMAX    := $(MINMAX:%.c=$(BIN)/%.o)

EVOLUTIVO := $(wildcard minmax/*.c)
EVOLUTIVO := $(MINMAX:%.c=$(BIN)/%.o)

# Util
.PHONY: subdirs all clean\
		run_jogar run_mapa run_minmax run_evolutivo\
		comp_mapa comp_minmax comp_evoltivo

subdirs:
	mkdir -p $(BIN)/mapa
	mkdir -p $(BIN)/minmax
	mkdir -p $(BIN)/modulos

### Acoes
all:
	mkdir -p $(F_BIN)
	$(MAKE) compile

clean:
	rm $(BIN)/modulos/*.o $(RMFLAG)
	rm $(BIN)/mapa/*.o $(RMFLAG)
	rm $(BIN)/minmax/*.o $(RMFLAG)

# Rodar Programa
run_jogo:
	cd jogo && ../venv/bin/python jogo.py

run_mapa:
	./mapa.out

run_minmax:
	./minmax.out

run_evolutivo:
	./evoltivo.out

# Compilar programa
comp_mapa: subdirs $(MODULOS) $(MAPA)
	$(CC) -o mapa.out $(MODULOS) $(MAPA) $(FLAGS)

comp_minmax: subdirs $(MODULOS) $(MINMAX)
	$(CC) -o minmax.out $(MODULOS) $(MINMAX) $(FLAGS)

comp_evoltivo: subdirs $(MODULOS) $(EVOLUTIVO)
	$(CC) -o evolutivo.out $(MODULOS) $(EVOLUTIVO) $(FLAGS)

### Compilando ...
$(BIN)/modulos/%.o: modulos/%.c
	$(CC) -c -o $@ $< $(FLAGS) 

$(BIN)/mapa/%.o: mapa/%.c
	$(CC) -c -o $@ $< $(FLAGS) -iquote modulos

$(BIN)/minmax/%.o: minmax/%.c
	$(CC) -c -o $@ $< $(FLAGS) -iquote modulos

$(BIN)/evoltivo/%.o: evoltivo/%.c
	$(CC) -c -o $@ $< $(FLAGS) -iquote modulos
