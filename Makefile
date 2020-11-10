### Compiladores
CC     := gcc

### Flags de Compilação
LIBS   :=
CFLAGS := $(LIBS)
FFLAGS := $(LIBS) -O3 -march=native -w
DFLAGS := $(LIBS) -g -Wall -Wextra -pedantic -Werror=implicit-function-declaration -fsanitize=address
FLAGS  := $(DFLAGS)

# Outras Flags
RMFLAG := -f -v

### Pasta dos .o
BIN    := bin

### Arquivos
MAPA   := $(wildcard mapa/*.c)
MAPA   := $(MAPA:%.c=$(BIN)/%.o)

MODULO := $(wildcard modulos/*.c)
MODULO := $(MODULOS:%.c=$(BIN)/%.o)

MINMAX := $(wildcard minmax/*.c)
MINMAX := $(MINMAX:%.c=$(BIN)/%.o)

# UTIL
.PHONY: subdirs all run jogar mapa minmax clean

subdirs:
	mkdir -p $(BIN)/mapa
	mkdir -p $(BIN)/minmax
	mkdir -p $(BIN)/modulos

### Ações
all:
	mkdir -p $(F_BIN)
	$(MAKE) compile

run: $(EXEC)
	./$(EXEC)


jogar:
	cd jogo && ../venv/bin/python jogo.py


### Compilação
mapa: $(MAPA)
	$(CC) -o mapa.out $(MAPA) $(FLAGS)

minmax: subdirs $(MODULOS) $(MINMAX)
	$(CC) -o minmax.out $(MODULOS) $(MINMAX) $(FLAGS)

### Clean
clean:
	rm $(BIN) $(RMFLAG)

### Exec
$(BIN)/mapa/%.o: mapa/%.c
	$(CC) -c -o $@ $< $(FLAGS)

$(BIN)/modulos/%.o: modulos/%.c
	$(CC) -c -o $@ $< $(FLAGS)

$(BIN)/minmax/%.o: minmax/%.c
	$(CC) -c -o $@ $< $(FLAGS) -I modulos
