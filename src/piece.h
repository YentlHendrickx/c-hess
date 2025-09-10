#ifndef PIECE_H
#define PIECE_H

#include "game.h"

void get_allowed_moves(piece_t piece, int start_row, int start_col, board_t* board, int possible_moves[8][8]);

#endif // PIECE_H
