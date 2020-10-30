#ifndef MAPA_H
#define MAPA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

extern int* mapa;
extern int tam_cromossomo;

bool carregar_mapa();
void free_mapa();

#endif