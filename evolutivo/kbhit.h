#ifndef KBHIT_H
#define KBHIT_H

#include <stdbool.h>
#include <sys/ioctl.h>
#include <termios.h>

void enable_raw_mode();
void disable_raw_mode();
bool kbhit();

#endif