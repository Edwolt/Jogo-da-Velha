#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define INF 0x3f3f3f3f
#define N 19683 // 
// Testando se eh viavel calcular todas as possibilidades do jogo da velha
// Define um número para cada posição possível no jogo da velha

int opcoes[8][9] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8},
    {2, 1, 0, 5, 4, 3, 8, 7, 6},
    {2, 5, 8, 1, 4, 7, 0, 3, 6},
    {8, 5, 2, 7, 4, 1, 6, 3, 0},
    {8, 7, 6, 5, 4, 3, 2, 1, 0},
    {6, 7, 8, 3, 4, 5, 0, 1, 2},
    {6, 3, 0, 7, 4, 1, 8, 5, 2},
    {0, 3, 6, 1, 4, 7, 2, 5, 8}
};

int pot(int base, int exp){
    int i;
    int val = 1;
    for (i = 0; i < exp; i++) {
        val *= base;
    }
    return val;
}

int min(int a, int b) {
    return (a < b ? a : b);
}

int define_num(int jogo[]){
    int i, j;
    int numero;
    int minimo = INF;

    for (i = 0; i < 8; i++) {
        numero = 0;
        for (j = 0; j < 9; j++) {
            numero += jogo[opcoes[i][j]] * pot(3, j);
        }
        minimo = min(numero, minimo);
    }
    return minimo;
}

int calc_num(int jogo[]){
    int i = 0;
    int numero = 0;
    for (i = 0; i < 9; i ++) numero += jogo[i] * pot(3, i);
    return numero;
}

bool proximo(int jogo[]){
    int i;

    for (i = 0; i < 9; i++) {
        jogo[i] += 1;
        if (jogo[i] > 2) jogo[i] = 0;
        else return true;
    }

    return false;
}

void imprime_jogo(int jogo[]){
    int i;
    for (i = 0; i < 8; i++) printf("%d ", jogo[i]);
    printf("%d", jogo[8]);

}

int main(){
    int i;
 
    int jogo[9];
    int num;
    int lista[N];

    for (i = 0; i < 9; i++) jogo[i] = 0;
    for (i = 0; i < N; i++) lista[i] = -1;

    do {
        num = define_num(jogo);
        imprime_jogo(jogo);
        printf(": %d [%d]\n", num, calc_num(jogo));
        lista[calc_num(jogo)] = num;
    } while (proximo(jogo));   
    printf("---\n");

    bool listado[N];
    for (int i = 0; i < N; i++) listado[i] = false;
    for (i = 0; i < N; i++) listado[lista[i]] = true;

    int contador = 0;
    for (i = 0; i < N; i++) contador += listado[i]; // if (listado[i]) contador++;
    printf("Número de valores único: %d\n", contador);
}