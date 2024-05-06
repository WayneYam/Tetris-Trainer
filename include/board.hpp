#ifndef BOARD_H
#define BOARD_H

#include "types.hpp"

void reset_board();
Board getBoard();
void init_piece(int c);
bool move_piece(int dir);
bool rotate_piece(int cnt);
void hard_drop();
void init_board();
void swap_piece();
int get_hold_piece();

#endif

