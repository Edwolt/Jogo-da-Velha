#include "util.h"

static const int N = 19683;   // powi(3, 9); numero de arranjos possiveis de X e O
static const int M = 986410;  //409114;  // 1 + 9 + 9*8 + 9*8*7 + ... + 9*8*7*6*5*4*3*2*1 ; Numemro maximo de recursoes de jogo_possiveis()

byte simetrias[8][9] = {{0, 1, 2, 3, 4, 5, 6, 7, 8},
                        {2, 1, 0, 5, 4, 3, 8, 7, 6},
                        {2, 5, 8, 1, 4, 7, 0, 3, 6},
                        {8, 5, 2, 7, 4, 1, 6, 3, 0},
                        {8, 7, 6, 5, 4, 3, 2, 1, 0},
                        {6, 7, 8, 3, 4, 5, 0, 1, 2},
                        {6, 3, 0, 7, 4, 1, 8, 5, 2},
                        {0, 3, 6, 1, 4, 7, 2, 5, 8}};

byte simetrias_reversa[8][9] = {{0, 1, 2, 3, 4, 5, 6, 7, 8},
                                {2, 1, 0, 5, 4, 3, 8, 7, 6},
                                {6, 3, 0, 7, 4, 1, 8, 5, 2},
                                {8, 5, 2, 7, 4, 1, 6, 3, 0},
                                {8, 7, 6, 5, 4, 3, 2, 1, 0},
                                {6, 7, 8, 3, 4, 5, 0, 1, 2},
                                {2, 5, 8, 1, 4, 7, 0, 3, 6},
                                {0, 3, 6, 1, 4, 7, 2, 5, 8}};

byte vitorias[8][3] = {{0, 1, 2},
                       {3, 4, 5},
                       {6, 7, 8},
                       {0, 3, 6},
                       {1, 4, 7},
                       {2, 5, 8},
                       {0, 4, 8},
                       {2, 4, 6}};

//* ===== Calc ===== *//

/**
 * Retorna quem venceu o jogo
 * 0: Ainda tem jogadas a ser feitas
 * 1: O jogador X ganhou
 * 2: O jogador O ganhou
 * 3: deu velha
 */
byte calc_vencedor(byte* jogo) {
    int i;
    byte a, b, c;
    for (i = 0; i < 8; i++) {
        a = vitorias[i][0];
        b = vitorias[i][1];
        c = vitorias[i][2];
        if (jogo[a] != 0 && jogo[a] == jogo[b] && jogo[b] == jogo[c]) return jogo[a];
    }

    for (i = 0; i < 9; i++) {
        if (jogo[i] == 0) return 0;
    }

    return 3;
}

/*
calc_min, calc_sim e calc_val usa simetrias_reversa e nao simetrias
a ideia é que eu passasse o jogo pela simetria e depois percorresse calculando o valor

for i in 0..8
    jogo_simetrico = aplica simetria[i] no jogo
    for j in 0..9
        numero += jogo_simetrico[j]

Para conseguir fazer isso sem ter que aplicar a simetria ao jogo
eu posso transformar a posicao j de uma posicao j dentro da simetria i
para a posicao equivalente no jogo especifico que foi passado,
e para isso usamos simtreias_reversa
*/

int calc_jogo(byte* jogo) {
    int i;
    register int numero = 0;
    for (i = 0; i < 9; i++) numero += jogo[i] * powi(3, i);
    return numero;
}

int calc_min(byte* jogo) {
    int i, j;
    int minimo = 3 * N;  // Nao tem como dar um numero maior que N
    register int numero;

    for (i = 0; i < 8; i++) {
        numero = 0;
        for (j = 0; j < 9; j++) numero += jogo[simetrias_reversa[i][j]] * powi(3, j);
        minimo = min(numero, minimo);
    }
    return minimo;
}

int calc_sim(byte* jogo) {
    int i, j;
    int minimo = 3 * N;  // Nao tem como dar um numero maior que N
    int sim = -1;
    register int numero;

    for (i = 0; i < 8; i++) {
        numero = 0;
        for (j = 0; j < 9; j++) numero += jogo[simetrias_reversa[i][j]] * powi(3, j);
        if (numero < minimo) {
            minimo = numero;
            sim = i;
        }
    }
    return sim;
}

int calc_val(byte* jogo, int sim) {
    int j;
    register int numero = 0;
    for (j = 0; j < 9; j++) numero += jogo[simetrias_reversa[sim][j]] * powi(3, j);
    return numero;
}

//* ===== Jogo ===== *//

/**
 * jogos deve comecar com o primeiro jogo com tudo 0
 * n deve comecar valendo 1 e sera usado para armazenar dados no vetor
 * Retorna se a operacao foi possivel
 */
static bool _jogos_possiveis(byte** jogos, int* n, int atual, int vez) {
    int i, k;
    bool ok;

    for (i = 0; i < 9; i++) {
        if (jogos[atual][i] != 0) continue;  // Essa posicao ja foi jogada
        k = (*n)++;

        // Cria uma copia do jogo atual
        jogos[k] = copiar_jogo(jogos[atual]);
        if (!jogos[k]) goto falha;

        // Faz jogada
        jogos[k][i] = vez;
        if (venceu_ou_velha(jogos[k])) {
            free(jogos[k]);
            jogos[k] = NULL;
            (*n)--;
            continue;
        };

        // Passa para a funcao fazer outra jogada
        ok = _jogos_possiveis(jogos, n, k, (vez == 1 ? 2 : 1));
        if (!ok) goto falha;
    }
    return true;

falha:
    return false;
}

byte** jogos_possiveis(int* n) {
    byte** jogos = NULL;
    int i;
    bool ok;

    // Aloca array de jogos
    jogos = malloc(M * sizeof(byte*));
    if (!jogos) goto falha;
    for (i = 0; i < M; i++) jogos[i] = NULL;

    // Aloca primeiro jogo e inicializa com tudo 0
    jogos[0] = malloc(9 * sizeof(byte));
    if (!jogos[0]) goto falha;
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

byte* copiar_jogo(byte* jogo) {
    byte* novo_jogo = NULL;
    int i;

    novo_jogo = malloc(9 * sizeof(byte));
    if (!novo_jogo) return NULL;

    for (i = 0; i < 9; i++) novo_jogo[i] = jogo[i];
    return novo_jogo;
}