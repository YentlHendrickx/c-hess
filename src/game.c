#include "game.h"
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

const int EMPTY = 0;
const int NONE = 0;

const int PAWN = 1;
const int KNIGHT = 2;
const int ROOK = 3;
const int BISHOP = 4;
const int QUEEN = 5;
const int KING = 6;

const int WHITE = 1;
const int BLACK = 2;

const int THEME_DEFAULT = 0;
const int THEME_WOOD = 1;

// Global game state
static struct GameState g_game_state = {0};

int init_game_state(void) {
  // Initialize board with empty pieces
  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      g_game_state.board.squares[row][col].piece.type = EMPTY;
      g_game_state.board.squares[row][col].piece.color = NONE;
      g_game_state.board.squares[row][col].piece.theme = THEME_DEFAULT;
      g_game_state.board.squares[row][col].row = row;
      g_game_state.board.squares[row][col].column = col;
    }
  }

  g_game_state.currentTurn = WHITE;
  g_game_state.selectedPieceRow = -1;
  g_game_state.selectedPieceCol = -1;

  // Clear possible moves
  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      g_game_state.possibleMoves[row][col] = 0;
    }
  }

  return ERROR_NONE;
}

void cleanup_game_state(void) {
  // Nothing to cleanup for now, but placeholder for future cleanup
}

int updateState(void) { return 0; }

// Validation functions
int is_valid_position(int row, int col) {
  return (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE);
}

int is_valid_piece_type(int type) { return (type >= EMPTY && type <= KING); }

int is_valid_color(int color) {
  return (color == NONE || color == WHITE || color == BLACK);
}

int is_valid_theme(int theme) {
  return (theme == THEME_DEFAULT || theme == THEME_WOOD);
}

// Board manipulation functions
struct Piece *get_piece_at(struct Board *board, int row, int col) {
  if (!board || !is_valid_position(row, col)) {
    return NULL;
  }
  return &board->squares[row][col].piece;
}

int set_piece_at(struct Board *board, int row, int col, struct Piece piece) {
  if (!board || !is_valid_position(row, col)) {
    return ERROR_INVALID_INPUT;
  }

  if (!is_valid_piece_type(piece.type) || !is_valid_color(piece.color) ||
      !is_valid_theme(piece.theme)) {
    return ERROR_INVALID_INPUT;
  }

  board->squares[row][col].piece = piece;
  return ERROR_NONE;
}

int clear_piece_at(struct Board *board, int row, int col) {
  if (!board || !is_valid_position(row, col)) {
    return ERROR_INVALID_INPUT;
  }

  board->squares[row][col].piece.type = EMPTY;
  board->squares[row][col].piece.color = NONE;
  board->squares[row][col].piece.theme = THEME_DEFAULT;

  return ERROR_NONE;
}
