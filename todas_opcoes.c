#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define INF 0x3f3f3f3f

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

int calc_num(int jogo[]){
    int i, j;
    int numero;
    int minimo = INF;

    for (i = 0; i < 8; i++) {
        numero = 0;
        for(j = 0; j < 9; j++){
            numero += jogo[opcoes[i][j]] * pot(3, j);
        }
        minimo = min(numero, minimo);
    }
    return minimo;
}

bool proximo(int jogo[]){
    int i;

    for (i = 0; i < 9; i++) {
        jogo[i] += 1;
        if (jogo[i] > 3) {
            jogo[i] = 0;
        } else {
            return true;
        }
    }

    return false;
}

int main(){
    int i;
 
    int jogo[9];
    int num;

    for (i = 0; i < 9; i++) {
        jogo[i] = 0;
    }

    do {
        for(int i = 0; i < 9; i++){
            printf("%d ", jogo[i]);
        }
        printf(": %d\n", calc_num(jogo));
    } while (proximo(jogo));
    printf("---\n");
}