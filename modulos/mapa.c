#include "mapa.h"

int* mapa = NULL;
int tam_cromossomo = 0;

bool carregar_mapa(char* path) {
    FILE* arquivo = NULL;
    int i;
    int tam_mapa;

    arquivo = fopen(path, "r");
    if (!arquivo) goto falha;

    fscanf(arquivo, "%d %d", &tam_mapa, &tam_cromossomo);
    if (ferror(arquivo)) goto falha;

    mapa = malloc(tam_mapa * sizeof(int));
    int dado;
    for (i = 0; i < tam_mapa; i++) {
        fscanf(arquivo, "%d", &dado);
        mapa[i] = dado;
    }

    fclose(arquivo);
    return true;

falha:
    fclose(arquivo);
    free_mapa(mapa);
    return false;
}

void free_mapa() {
    free(mapa);
    mapa = NULL;
}