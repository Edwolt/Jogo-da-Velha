#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

// Obs: Se X for 1 e O 2 entao eu vou ter mais 1 que 2 no numero ternario, o que vai criar um mapa menor

static const int N = 19683;   // powi(3, 9); numero de arranjos possiveis de X e O
static const int M = 986410;  //409114;  // 1 + 9 + 9*8 + 9*8*7 + ... + 9*8*7*6*5*4*3*2*1 ; Numemro maximo de recursoes de jogo_possiveis()

// extern int simetrias[8][9];
extern int simetrias_reversa[8][9];
extern int vitorias[8][3];

/**
 * Salva mapa em uma arquivo em path
 * Retorna se a operacao foi possivel
 */
static bool salva_mapa(char* path, int* mapa, int tam_mapa, int tam_cromossomo) {
    if (!path || !mapa) return false;

    FILE* arquivo = NULL;
    int i;

    arquivo = fopen(path, "w");
    if (!arquivo) goto falha;

    fprintf(arquivo, "%d %d\n", tam_mapa, tam_cromossomo);
    if (ferror(arquivo)) goto falha;

    for (i = 0; i < tam_mapa; i++) {
        fprintf(arquivo, "%d\n", mapa[i]);
        if (ferror(arquivo)) goto falha;
    }

    fclose(arquivo);
    return true;

falha:
    fclose(arquivo);
    return false;
}

/**
 * Gera um mapa e salva em mapa.txt
 * Esse mapa serve para mapear todos os possivei jogos
 * eliminando jogos simetricos e fazendo com que o vetor
 * com todas as jogadas a ser feita seja menor
 */
int main() {
    int** jogos = NULL;

    int i;
    bool ok;
    int m;
    int minimos[M];          // 0..M -> 0..N
    bool freq[N];            //
    int mapa[N];             // 0..N -> 0..tam_cromossomo
    int tam_mapa = -1;       // Maior valor entre os minimos
    int tam_cromossomo = 0;  // Numero de valores unicos em minimos

    for (i = 0; i < M; i++) minimos[i] = -1;
    for (i = 0; i < N; i++) freq[i] = false;
    for (i = 0; i < N; i++) mapa[i] = -1;

    jogos = jogos_possiveis(&m);
    if (!jogos) goto falha;

    // Calcula os minimos de cada jogo
    for (i = 0; i < m; i++) minimos[i] = calc_min(jogos[i]);

    for (i = 0; i < m; i++) {
        tam_mapa = max(minimos[i], tam_mapa);
        freq[minimos[i]] = true;
    }
    tam_mapa++;  // Tem que incluir o maior tambem
    for (i = 0; i < N; i++) tam_cromossomo += freq[i];

    // Calcula mapa
    int acc = 0;
    for (i = 0; i < N; i++) {
        if (freq[i]) {
            mapa[i] = acc;
            acc += 1;
        }
    }

    // Salva o mapa e um arquivo
    ok = salva_mapa("mapa.txt", mapa, tam_mapa, tam_cromossomo);
    if (!ok) goto falha;

    // Desalocando memória
    for (i = 0; i < M; i++) free(jogos[i]);
    free(jogos);

    printf("Mapa salvo em mapa.txt\n");
    printf("Tamanho do mapa: %d\n", tam_mapa);
    printf("Tamanho do cromossomo: %d\n", tam_cromossomo);
    return EXIT_SUCCESS;

falha:
    if (jogos) {
        for (i = 0; i < M; i++) free(jogos[i]);
        free(jogos);
    }
    printf("Falha ao criar mapa\n");
    return EXIT_FAILURE;
}