#ifndef PIECE_H
#define PIECE_H

#include "game.h"

// Piece types and colors
extern const int EMPTY;
extern const int NONE;

extern const int PAWN;
extern const int KNIGHT;
extern const int BISHOP;
extern const int ROOK;
extern const int QUEEN;
extern const int KING;

extern const int BLACK;
extern const int WHITE;

extern const int THEME_DEFAULT;
extern const int THEME_WOOD;

void get_allowed_moves(game_state_t *game_state, int start_row, int start_col);

int is_valid_position(int row, int col);
int is_valid_piece_type(int type);
int is_valid_color(int color);
int is_valid_theme(int theme);

// Board manipulation functions
piece_t* get_piece_at(board_t* board, int row, int col);
int set_piece_at(board_t* board, int row, int col, piece_t piece);
int clear_piece_at(board_t* board, int row, int col);

#endif // PIECE_H
