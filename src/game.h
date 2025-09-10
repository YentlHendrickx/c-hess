#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "config.h"
#include "input.h"

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

struct piece_t {
  int type; // 0 = empty, 1 = pawn, 2 = knight, etc.
  int color; // 0 = none, 1 = white, 2 = black
  int theme; // 0 = standard, 1 = wood
};

struct square_t {
  struct piece_t piece;
  int column; // 0-7
  int row;    // 0-7
};

typedef struct {
  struct square_t squares[8][8];
} board_t;

typedef struct {
  board_t board;
  int current_turn; // 1 = white, 2 = black
  int move_count;
  int game_over; // 0 = ongoing, 1 = white wins, 2 = black wins, 3 = draw
  int render_needed; // 1 = needs re-render, 0 = no changes
  int selected_piece_row;
  int selected_piece_col;
  int possible_moves[8][8]; // 1 = possible move, 0 = not possible
  input_state_t *input_state;
} game_state_t;

// Game state functions
void update_state(SDL_Surface* screen);
game_state_t* init_game_state(void);
void cleanup_game_state(void);

// Validation functions
int is_valid_position(int row, int col);
int is_valid_piece_type(int type);
int is_valid_color(int color);
int is_valid_theme(int theme);

// Board manipulation functions
struct piece_t* get_piece_at(board_t* board, int row, int col);
int set_piece_at(board_t* board, int row, int col, struct piece_t piece);
int clear_piece_at(board_t* board, int row, int col);

#endif // GAME_H
