### Compiladores
CC       := gcc

### Output
EXEC     := mapa.out

### Pastas
F_UTIL := util
F_SRC  := .
F_BIN  := bin

### Arquivos
INCLUDES := $(wildcard $(F_SRC)/optional/*.c) $(wildcard $(F_SRC)/optional/*.h)
SRC      := $(wildcard $(F_SRC)/*.c)
OBJ      := $(SRC:$(F_SRC)/%.c=$(F_BIN)/%.o)
HEADER   := $(wildcard $(F_SRC)/*.h)
MK       := Makefile


### Flags de Compilação
LIBS     :=
CFLAGS   := $(LIBS)
FFLAGS   := $(LIBS) -O3 -march=native -w
DFLAGS   := $(LIBS) -g -Wall -Wextra -pedantic -Werror=implicit-function-declaration -fsanitize=address

FLAGS := $(DFLAGS)

# Outras Flags
RMFLAGS  := -f -v

.PHONY: all run jogar compile mapa clean

### Ações
all:
	mkdir -p $(F_BIN)
	$(MAKE) compile

run: $(EXEC)
	./$(EXEC)


jogar:
	cd jogo && ../venv/bin/python jogo.py

### Compilação
compile: clean $(EXEC)

mapa:
	$(CC) -o mapa.out mapa/mapa.c $(FLAGS)
	./mapa.out

### Clean
clean:
	rm $(OBJ) $(RMFLAGS)
	rm $(DEBUG) $(RMFLAGS)
	rm $(EXEC) $(RMFLAGS)


### Exec
$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(FLAGS)

$(F_BIN)/%.o: $(F_SRC)/%.c
	$(CC) -c -o $@ $< $(FLAGS)