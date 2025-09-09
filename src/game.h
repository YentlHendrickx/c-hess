#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "config.h"

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

struct Piece {
  int type; // 0 = empty, 1 = pawn, 2 = knight, etc.
  int color; // 0 = none, 1 = white, 2 = black
  int theme; // 0 = standard, 1 = wood
};

struct Square {
  struct Piece piece;
  int column; // 0-7
  int row;    // 0-7
};

struct Board {
  struct Square squares[8][8];
};

struct GameState {
  struct Board board;
  int currentTurn; // 1 = white, 2 = black
  int selectedPieceRow;
  int selectedPieceCol;
  int possibleMoves[8][8]; // 1 = possible move, 0 = not possible
};

// Game state functions
int updateState(void);
int init_game_state(void);
void cleanup_game_state(void);

// Validation functions
int is_valid_position(int row, int col);
int is_valid_piece_type(int type);
int is_valid_color(int color);
int is_valid_theme(int theme);

// Board manipulation functions
struct Piece* get_piece_at(struct Board* board, int row, int col);
int set_piece_at(struct Board* board, int row, int col, struct Piece piece);
int clear_piece_at(struct Board* board, int row, int col);

#endif // GAME_H
