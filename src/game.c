#include "game.h"
#include "renderer.h"
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
static game_state_t g_game_state = {0};

void init_standard_board(void) {
  int initial_positions[8][8] = {
      {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK},
      {PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN},
      {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
      {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
      {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
      {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
      {PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN},
      {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK}};

  for (int col = 0; col < BOARD_SIZE; col++) {
    /// WHITE
    g_game_state.board.squares[7][col].piece.type = initial_positions[0][col];
    g_game_state.board.squares[7][col].piece.color = WHITE;

    // Pawns
    g_game_state.board.squares[6][col].piece.type = initial_positions[1][col];
    g_game_state.board.squares[6][col].piece.color = WHITE;

    /// BLACK
    g_game_state.board.squares[0][col].piece.type = initial_positions[7][col];
    g_game_state.board.squares[0][col].piece.color = BLACK;

    // Pawns
    g_game_state.board.squares[1][col].piece.type = initial_positions[6][col];
    g_game_state.board.squares[1][col].piece.color = BLACK;
  }
}

game_state_t *init_game_state(void) {
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

  g_game_state.current_turn = WHITE;
  g_game_state.selected_piece_row = -1;
  g_game_state.selected_piece_col = -1;
  g_game_state.game_over = 0;     // Game ongoing
  g_game_state.move_count = -1;   // Indicates game just started
  g_game_state.render_needed = 1; // Initial render needed
  g_game_state.input_state = malloc(sizeof(input_state_t));

  // Clear possible moves
  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      g_game_state.possible_moves[row][col] = 0;
    }
  }

  // Assign standard chess starting positions
  init_standard_board();

  return &g_game_state;
}

void cleanup_game_state(void) {
  // Nothing to cleanup for now, but placeholder for future cleanup
}

void find_piece_by_coordinates(SDL_Surface *screen, int x, int y) {
  int cell_size = get_cell_size(screen);
  int col = x / cell_size;
  int row = y / cell_size;

  if (is_valid_position(row, col)) {
    struct piece_t *piece = get_piece_at(&g_game_state.board, row, col);
    if (piece && piece->type != EMPTY) {
      g_game_state.selected_piece_row = row;
      g_game_state.selected_piece_col = col;
      printf("Selected piece at (%d, %d): type %d, color %d\n", row, col,
             piece->type, piece->color);
    } else {
      g_game_state.selected_piece_row = -1;
      g_game_state.selected_piece_col = -1;
      printf("No piece at (%d, %d)\n", row, col);
    }

    return;
  }

  g_game_state.selected_piece_row = -1;
  g_game_state.selected_piece_col = -1;
  printf("Clicked outside board at (%d, %d)\n", x, y);
}

void handle_mouse_click(SDL_Surface *screen, int x, int y) {
  printf("Mouse clicked at (%d, %d)\n", g_game_state.input_state->mouse_x,
         g_game_state.input_state->mouse_y);
  find_piece_by_coordinates(screen, x, y);
  if (g_game_state.selected_piece_row != -1 &&
      g_game_state.selected_piece_col != -1) {
    printf("Calculating possible moves for selected piece\n");

    // TODO: Implement real move calculation logic
    // For now, just mark all squares as possible moves around the piece
    int sel_row = g_game_state.selected_piece_row;
    int sel_col = g_game_state.selected_piece_col;
    for (int row = 0; row < BOARD_SIZE; row++) {
      for (int col = 0; col < BOARD_SIZE; col++) {
        // Simple logic: mark adjacent squares as possible moves
        if ((abs(row - sel_row) <= 1) && (abs(col - sel_col) <= 1) &&
            !(row == sel_row && col == sel_col)) {
          g_game_state.possible_moves[row][col] = 1;
        } else {
          g_game_state.possible_moves[row][col] = 0;
        }
      }
    }
  } else {
    printf("No piece selected, clearing possible moves\n");
    // Clear possible moves if no piece selected
    for (int row = 0; row < BOARD_SIZE; row++) {
      for (int col = 0; col < BOARD_SIZE; col++) {
        g_game_state.possible_moves[row][col] = 0;
      }
    }
  }

  g_game_state.input_state->mouse_clicked = 0; // Reset click state
}

void update_state(SDL_Surface *screen) {
  if (g_game_state.move_count == -1) {
    printf("Game started. White's turn.\n");
    g_game_state.move_count++;
  }

  if (g_game_state.input_state->mouse_clicked) {
    handle_mouse_click(screen, g_game_state.input_state->mouse_x,
                       g_game_state.input_state->mouse_y);
    g_game_state.render_needed = 1; // Mark for re-render
  }
}

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
struct piece_t *get_piece_at(board_t *board, int row, int col) {
  if (!board || !is_valid_position(row, col)) {
    return NULL;
  }
  return &board->squares[row][col].piece;
}

int set_piece_at(board_t *board, int row, int col, struct piece_t piece) {
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

int clear_piece_at(board_t *board, int row, int col) {
  if (!board || !is_valid_position(row, col)) {
    return ERROR_INVALID_INPUT;
  }

  board->squares[row][col].piece.type = EMPTY;
  board->squares[row][col].piece.color = NONE;
  board->squares[row][col].piece.theme = THEME_DEFAULT;

  return ERROR_NONE;
}
