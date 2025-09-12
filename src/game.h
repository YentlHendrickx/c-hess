#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "config.h"
#include "input.h"

typedef struct {
  int type; // 0 = empty, 1 = pawn, 2 = knight, etc.
  int color; // 0 = none, 1 = white, 2 = black
  int theme; // 0 = standard, 1 = wood
} piece_t;

typedef struct {
  piece_t piece;
  int column; // 0-7
  int row;    // 0-7
} square_t;

typedef struct {
  square_t squares[8][8];
} board_t;

typedef struct {
  int from_row;
  int from_col;
  int to_row;
  int to_col;
  piece_t moved_piece;
  piece_t captured_piece; // type = EMPTY if no capture
  int is_castling; // 1 = castling move, 0 = normal move
  int is_en_passant; // 1 = en passant capture, 0 = normal move
  int is_promotion; // 1 = pawn promotion, 0 = normal move
  int promotion_piece; // piece type for promotion (if is_promotion = 1)
} move_history_t;

typedef struct {
  board_t board;
  int current_turn; // 1 = white, 2 = black
  int move_count;
  int game_over; // 0 = ongoing, 1 = white wins, 2 = black wins, 3 = draw
  int render_needed; // 1 = needs re-render, 0 = no changes
  int selected_piece_row;
  int selected_piece_col;
  int possible_moves[8][8]; // 1 = possible move, 0 = not possible
  move_history_t move_list[1024]; // Store up to 1024 moves; realistically this is more than enough
  input_state_t *input_state;
} game_state_t;

// Game state functions
void update_state(SDL_Renderer* renderer);
game_state_t* init_game_state(void);
void cleanup_game_state(void);
int undo_last_move(void);
void print_last_moves(int count);
void print_help(void);

#endif // GAME_H
