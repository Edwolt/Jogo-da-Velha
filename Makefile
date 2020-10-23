### Copilation
CC       := gcc

### Output
EXEC     := mapa.out
ZIP      := $(EXEC).zip

### Folder
F_UTIL := util
F_SRC  := .
F_BIN  := bin

### Files
INCLUDES := $(wildcard $(F_SRC)/optional/*.c) $(wildcard $(F_SRC)/optional/*.h)
SRC      := $(wildcard $(F_SRC)/*.c)
OBJ      := $(SRC:$(F_SRC)/%.c=$(F_BIN)/%.o)
HEADER   := $(wildcard $(F_SRC)/*.h)
MK       := Makefile


### Flags
LIBS     :=
CFLAGS   := $(LIBS)
FFLAGS   := $(LIBS) -O3 -march=native -w
DFLAGS   := $(LIBS) -g -Wall -Wextra -pedantic -Werror=implicit-function-declaration -fsanitize=address
RMFLAGS  := -f -v

### Actions
all:
	mkdir -p $(F_BIN)
	$(MAKE) compile

run: $(EXEC)
	./$(EXEC)


### Compile
compile: FLAGS := $(CFLAGS)
compile: clean $(EXEC)

final: FLAGS := $(FFLAGS)
final: clean $(EXEC)

debug: FLAGS := $(DFLAGS)
debug: clean $(EXEC)

### Clean
clean:
	rm $(OBJ) $(RMFLAGS)
	rm $(DEBUG) $(RMFLAGS)
	rm $(EXEC) $(RMFLAGS)
	rm $(ZIP) $(RMFLAGS)

clean_util:
	cd $(F_UTIL) && $(MAKE) clean

### Util
test:
	sh test.sh $(EXEC)

lines:
	sh lines.sh

util:
	cd $(F_UTIL) && $(MAKE) all

### Exec
$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(FLAGS)

$(F_BIN)/%.o: $(F_SRC)/%.c
	$(CC) -c -o $@ $< $(FLAGS)