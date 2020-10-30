#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Obs: Se X for 1 e O 2 entao eu vou ter mais 1 que 2 no numero ternario, o que vai criar um mapa menor

const int N = 19683;   // powi(3, 9); numero de arranjos possiveis de X e O
const int M = 986410;  //409114;  // 1 + 9 + 9*8 + 9*8*7 + ... + 9*8*7*6*5*4*3*2*1 ; Numemro maximo de recursoes de jogo_possiveis()

static int simetrias[8][9] = {{0, 1, 2, 3, 4, 5, 6, 7, 8},
                              {2, 1, 0, 5, 4, 3, 8, 7, 6},
                              {2, 5, 8, 1, 4, 7, 0, 3, 6},
                              {8, 5, 2, 7, 4, 1, 6, 3, 0},
                              {8, 7, 6, 5, 4, 3, 2, 1, 0},
                              {6, 7, 8, 3, 4, 5, 0, 1, 2},
                              {6, 3, 0, 7, 4, 1, 8, 5, 2},
                              {0, 3, 6, 1, 4, 7, 2, 5, 8}};

static int vitorias[8][3] = {{0, 1, 2},
                             {3, 4, 5},
                             {6, 7, 8},
                             {0, 3, 6},
                             {1, 4, 7},
                             {2, 5, 8},
                             {0, 4, 8},
                             {2, 4, 6}};

inline static int min(int a, int b) { return (a < b ? a : b); }
inline static int max(int a, int b) { return (a > b ? a : b); }

/**
 * Retorna x elevado a
 */
static int powi(int x, int n) {
    int y = 1;
    while (n > 0) {
        if (n % 2 != 0) y *= x;
        x *= x;
        n /= 2;
    }
    return y;
}

/**
 * Retorna o menor valor entre os jogos e seus simetricos
 */
static int calc_min(int* jogo) {
    int i, j;
    int minimo = 3 * N;  // Nao tem como dar um numero maior que N
    int numero;

    for (i = 0; i < 8; i++) {
        numero = 0;
        for (j = 0; j < 9; j++) numero += jogo[simetrias[i][j]] * powi(3, j);
        minimo = min(numero, minimo);
    }
    return minimo;
}

static bool venceu(int* jogo) {
    int i;
    int a, b, c;
    for (i = 0; i < 8; i++) {
        a = vitorias[i][0];
        b = vitorias[i][1];
        c = vitorias[i][2];
        if (jogo[a] != 0 && jogo[a] == jogo[b] && jogo[b] == jogo[c]) return true;
    }
    return false;
}

/**
 * jogos deve comecar com o primeiro jogo com tudo 0
 * n deve comecar valendo 1 e sera usado para armazenar dados no vetor
 * Retorna se a operacao foi possivel
 */
static bool _jogos_possiveis(int** jogos, int* n, int atual, int vez) {
    int i, j, k;
    bool ok;

    for (i = 0; i < 9; i++) {
        if (jogos[atual][i] != 0) continue;  // Essa posicao ja foi jogada
        k = (*n)++;

        // Cria uma copia do jogo atual
        jogos[k] = malloc(9 * sizeof(int));
        if (!jogos[k]) goto falha;
        for (j = 0; j < 9; j++) jogos[k][j] = jogos[atual][j];

        // Faz jogada
        jogos[k][i] = vez;
        if (venceu(jogos[k])) continue;

        // Passa para a funcao fazer outra jogada
        ok = _jogos_possiveis(jogos, n, k, (vez == 1 ? 2 : 1));
        if (!ok) goto falha;
    }
    return true;

falha:
    return false;
}

/**
 * Retorna em jogos uma lista de todos os jogos possiveis de acontecer
 * Retorna em n quantas posicoes foram usadas
 */
static inline int** jogos_possiveis(int* n) {
    int i;
    int** jogos = NULL;
    bool ok;

    // Aloca array de jogos
    jogos = malloc(M * sizeof(int*));
    if (!jogos) goto falha;
    for (i = 0; i < M; i++) jogos[i] = NULL;

    // Aloca primeiro jogo e inicializa com tudo 0
    jogos[0] = malloc(9 * sizeof(int));
    for (i = 0; i < 9; i++) jogos[0][i] = 0;

    // Chama a funcao auxiliar
    *n = 1;
    ok = _jogos_possiveis(jogos, n, 0, 1);
    if (!ok) goto falha;

    // Retorna os jogos encontrados
    return jogos;

falha:
    if (jogos) {
        for (i = 0; i < M; i++) free(jogos[i]);
        free(jogos);
    }
    return NULL;
}

static void salva_mapa(int* mapa, int n, int m) {
    int i;
    FILE* arquivo = fopen("mapa.txt", "w");
    if (!arquivo) return;

    fprintf(arquivo, "%d %d\n", n, m);
    for (i = 0; i < n; i++) fprintf(arquivo, "%d\n", mapa[i]);
    fclose(arquivo);
}

int main() {
    int i;

    int m;
    int** jogos = jogos_possiveis(&m);
    int minimos[M];  // 0..M -> 0..N
    for (i = 0; i < M; i++) minimos[i] = -1;
    bool freq[N];
    for (i = 0; i < N; i++) freq[i] = false;
    int mapa[N];  // 0..N -> 0..tam_cromossomo
    for (i = 0; i < N; i++) mapa[i] = -1;
    int tam_mapa = -1;       // Maior valor entre os minimos
    int tam_cromossomo = 0;  // Numero de valores unicos em minimos

    if (!jogos) goto falha;

    // Calcula os minimos de cada jogo
    for (i = 0; i < m; i++) minimos[i] = calc_min(jogos[i]);

    for (i = 0; i < m; i++) {
        tam_mapa = max(minimos[i], tam_mapa);
        freq[minimos[i]] = true;
    }
    tam_mapa++;  // Tem que incluir o maior tambem
    for (i = 0; i < N; i++) tam_cromossomo += freq[i];
    printf("Tam mapa: %d; Tam cromossomo %d\n", tam_mapa, tam_cromossomo);

    int acc = 0;
    for (i = 0; i < N; i++) {
        if (freq[i]) {
            mapa[i] = acc;
            acc += 1;
        }
        printf("%d -> %d\n", i, mapa[minimos[i]]);
    }

    salva_mapa(mapa, tam_mapa, tam_cromossomo);

    for (i = 0; i < M; i++) free(jogos[i]);
    free(jogos);
    return EXIT_SUCCESS;

falha:
    if (jogos) {
        for (i = 0; i < M; i++) free(jogos[i]);
        free(jogos);
    }
    printf("Mapeamento falhou\n");
    return EXIT_FAILURE;
}