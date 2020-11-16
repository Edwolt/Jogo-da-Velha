#ifndef KBHIT_H
#define KBHIT_H

#include <stdbool.h>
#include <sys/ioctl.h>
#include <termios.h>

/**
 * Usar antes de um trecho de codigo que use o kbhit
 */
void enable_raw_mode();

/**
 * Usar depois de um trecho de codigo que use o kbhit
 */
void disable_raw_mode();

/**
 * Verifica se uma tecla foi pressionada sem ler ela
 */
bool kbhit();

#endif