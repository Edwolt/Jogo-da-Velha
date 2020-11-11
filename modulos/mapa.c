#include "mapa.h"

int* mapa = NULL;
int tam_cromossomo = 0;

bool carregar_mapa(char* path) {
    FILE* file = fopen(path, "r");
    if (!file) return false;

    int i;
    int tam_mapa;
    fscanf(file, "%d %d", &tam_mapa, &tam_cromossomo);

    mapa = malloc(tam_mapa * sizeof(int));
    int dado;
    for (i = 0; i < tam_mapa; i++) {
        fscanf(file, "%d", &dado);
        mapa[i] = dado;
    }

    fclose(file);
    return true;
}

void free_mapa() {
    free(mapa);
    mapa = NULL;
}