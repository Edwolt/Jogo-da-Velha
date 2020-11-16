#ifndef MAPA_H
#define MAPA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Use o metodo carregar_mapa() para carregar o mapa na memoria
 */
extern int* mapa;

/**
 * Tamanho do cromossomo de mapa
 */
extern int tam_cromossomo;

/**
 * Carrega o mapa na memoria
 */
bool carregar_mapa(char* path);

/**
 * Desaloca o mapa da memoria
 */
void free_mapa();

#endif