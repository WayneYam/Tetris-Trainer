#ifndef MOTION_H
#define MOTION_H

#include "config.hpp"
#include "board.hpp"

void motion_register(Keybind k, int status, Board &B);
void do_motion(Board &B);

#endif

