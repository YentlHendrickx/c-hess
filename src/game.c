#include "game.h"
#include "piece.h"
#include "renderer.h"
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

static game_state_t g_game_state = {0};

char col_to_file(int col) { return 'a' + col; }
int row_to_rank(int row) { return 8 - row; }

char get_piece_symbol(int piece_type) {
  if (piece_type == PAWN)
    return '\0'; // Pawns have no symbol in notation
  if (piece_type == KING)
    return 'K'; // King symbol
  if (piece_type == QUEEN)
    return 'Q'; // Queen symbol
  if (piece_type == ROOK)
    return 'R'; // Rook symbol
  if (piece_type == BISHOP)
    return 'B'; // Bishop symbol
  if (piece_type == KNIGHT)
    return 'N'; // Knight symbol
  return '\0';  // Unknown piece
}

// Convert move to chess notation
void move_to_notation(move_history_t *move, char *notation, int max_len) {
  if (!move || !notation)
    return;

  char piece_symbol = get_piece_symbol(move->moved_piece.type);
  char from_file = col_to_file(move->from_col);
  char to_file = col_to_file(move->to_col);
  int to_rank = row_to_rank(move->to_row);

  // Handle special moves first
  if (move->is_castling) {
    if (move->to_col > move->from_col) {
      snprintf(notation, max_len, "O-O"); // Kingside castling
    } else {
      snprintf(notation, max_len, "O-O-O"); // Queenside castling
    }
    return;
  }

  if (move->is_promotion) {
    char promotion_symbol = get_piece_symbol(move->promotion_piece);
    if (move->captured_piece.type != EMPTY) {
      // Pawn capture with promotion (e.g., exd8=Q)
      snprintf(notation, max_len, "%cx%d=%c", from_file, to_rank,
               promotion_symbol);
    } else {
      // Pawn move with promotion (e.g., e8=Q)
      snprintf(notation, max_len, "%c%d=%c", to_file, to_rank,
               promotion_symbol);
    }
    return;
  }

  if (move->is_en_passant) {
    // En passant capture (e.g., exd6 e.p.)
    snprintf(notation, max_len, "%cx%d e.p.", from_file, to_rank);
    return;
  }

  // Regular moves
  if (move->captured_piece.type != EMPTY) {
    // Capture move
    if (move->moved_piece.type == PAWN) {
      // Pawn capture (e.g., exd5)
      snprintf(notation, max_len, "%cx%d", from_file, to_rank);
    } else {
      // Piece capture (e.g., Nxf7)
      snprintf(notation, max_len, "%c%c%d", piece_symbol, to_file, to_rank);
    }
  } else {
    // Normal move
    if (move->moved_piece.type == PAWN) {
      // Pawn move (e.g., e4)
      snprintf(notation, max_len, "%c%d", to_file, to_rank);
    } else {
      // Piece move (e.g., Nf3)
      snprintf(notation, max_len, "%c%c%d", piece_symbol, to_file, to_rank);
    }
  }
}

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

void find_square_by_coordinates(SDL_Renderer *renderer, int x, int y, int *row,
                                int *col) {
  int cell_size = get_cell_size(renderer);
  int temp_col = x / cell_size;
  int temp_row = y / cell_size;

  if (is_valid_position(temp_row, temp_col)) {
    *row = temp_row;
    *col = temp_col;
    return;
  }

  *row = -1;
  *col = -1;
}

void find_piece_by_square(int row, int col) {
  if (is_valid_position(row, col)) {
    piece_t *piece = get_piece_at(&g_game_state.board, row, col);
    if (piece && piece->type != EMPTY) {
      g_game_state.selected_piece_row = row;
      g_game_state.selected_piece_col = col;
      return;
    }

    g_game_state.selected_piece_row = -1;
    g_game_state.selected_piece_col = -1;
    printf("No piece at (%d, %d)\n", row, col);
  }

  g_game_state.selected_piece_row = -1;
  g_game_state.selected_piece_col = -1;
}

void clear_possible_moves(void) {
  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      g_game_state.possible_moves[row][col] = 0;
    }
  }
}

int try_make_move(int row, int col) {
  piece_t *selected_piece =
      get_piece_at(&g_game_state.board, g_game_state.selected_piece_row,
                   g_game_state.selected_piece_col);
  if (!selected_piece) {
    return 0; // No piece selected
  }

  // Get the piece that will be captured (if any)
  piece_t *captured_piece = get_piece_at(&g_game_state.board, row, col);
  piece_t captured_piece_copy = {0};
  if (captured_piece && captured_piece->type != EMPTY) {
    captured_piece_copy = *captured_piece;
  }

  // Record move in history BEFORE making the move
  g_game_state.move_list[g_game_state.move_count].from_row =
      g_game_state.selected_piece_row;
  g_game_state.move_list[g_game_state.move_count].from_col =
      g_game_state.selected_piece_col;
  g_game_state.move_list[g_game_state.move_count].to_row = row;
  g_game_state.move_list[g_game_state.move_count].to_col = col;
  g_game_state.move_list[g_game_state.move_count].moved_piece = *selected_piece;
  g_game_state.move_list[g_game_state.move_count].captured_piece =
      captured_piece_copy;

  // Initialize special move flags
  g_game_state.move_list[g_game_state.move_count].is_castling = 0;
  g_game_state.move_list[g_game_state.move_count].is_en_passant = 0;
  g_game_state.move_list[g_game_state.move_count].is_promotion = 0;
  g_game_state.move_list[g_game_state.move_count].promotion_piece = EMPTY;

  // Detect special moves
  if (selected_piece->type == KING &&
      abs(row - g_game_state.selected_piece_row) == 0 &&
      abs(col - g_game_state.selected_piece_col) == 2) {
    // Castling move detected
    g_game_state.move_list[g_game_state.move_count].is_castling = 1;
    printf("Castling move detected.\n");
  }

  if (selected_piece->type == PAWN &&
      ((selected_piece->color == WHITE && row == 0) ||
       (selected_piece->color == BLACK && row == 7))) {
    // Pawn promotion detected
    g_game_state.move_list[g_game_state.move_count].is_promotion = 1;
    g_game_state.move_list[g_game_state.move_count].promotion_piece =
        QUEEN; // Default to queen
    printf("Pawn promotion to Queen.\n");
  }

  if (selected_piece->type == PAWN && captured_piece_copy.type == EMPTY &&
      col != g_game_state.selected_piece_col) {
    // En passant capture detected
    g_game_state.move_list[g_game_state.move_count].is_en_passant = 1;
    printf("En passant capture detected.\n");

    // Clear the captured pawn
    int captured_row = (selected_piece->color == WHITE) ? row + 1 : row - 1;
    captured_piece_copy = *captured_piece;

    g_game_state.move_list[g_game_state.move_count].captured_piece =
        *get_piece_at(&g_game_state.board, captured_row, col);
    clear_piece_at(&g_game_state.board, captured_row, col);
  }

  // Move piece
  set_piece_at(&g_game_state.board, row, col, *selected_piece);
  clear_piece_at(&g_game_state.board, g_game_state.selected_piece_row,
                 g_game_state.selected_piece_col);

  // Switch turn
  g_game_state.current_turn =
      (g_game_state.current_turn == WHITE) ? BLACK : WHITE;
  g_game_state.move_count++;
  printf("It's now %s's turn.\n",
         (g_game_state.current_turn == WHITE) ? "White" : "Black");

  // Clear selection and possible moves
  g_game_state.selected_piece_row = -1;
  g_game_state.selected_piece_col = -1;
  clear_possible_moves();
  return 1;
}

void handle_mouse_click(SDL_Renderer *renderer, int x, int y) {
  g_game_state.input_state->mouse_clicked = 0; // Reset click state

  int row, col;
  find_square_by_coordinates(renderer, x, y, &row, &col);

  if (row == -1 || col == -1) {
    clear_possible_moves();
    return;
  }

  // We clicked on an empty square or invalid square, if this is already in
  // the possible moves list; and is really a possible move, move the piece
  if (g_game_state.possible_moves[row][col] == 1 &&
      g_game_state.selected_piece_row != -1 &&
      g_game_state.selected_piece_col != -1) {
    if (try_make_move(row, col))
      return;
  }

  find_piece_by_square(row, col);
  if (g_game_state.selected_piece_row != -1 &&
      g_game_state.selected_piece_col != -1) {

    if (g_game_state.board.squares[row][col].piece.color !=
        g_game_state.current_turn) {
      // Selected piece is not of the current player's color; ignore
      g_game_state.selected_piece_row = -1;
      g_game_state.selected_piece_col = -1;
      clear_possible_moves();
      return;
    }

    int sel_row = g_game_state.selected_piece_row;
    int sel_col = g_game_state.selected_piece_col;

    get_allowed_moves(&g_game_state, sel_row, sel_col);
    return;
  }

  // Clicked on empty square not in possible moves; clear selection
  g_game_state.selected_piece_row = -1;
  g_game_state.selected_piece_col = -1;
  clear_possible_moves();
}

void print_history(void) {
  if (g_game_state.move_count <= 0) {
    printf("No moves made yet.\n");
    return;
  }

  printf("\n=== Move History ===\n");
  for (int i = 0; i < g_game_state.move_count; i++) {
    move_history_t *move = &g_game_state.move_list[i];
    char notation[16];
    move_to_notation(move, notation, sizeof(notation));

    // Print move number and notation
    if (i % 2 == 0) {
      // White's move
      printf("%d. %s", (i / 2) + 1, notation);
    } else {
      // Black's move
      printf(" %s\n", notation);
    }
  }

  // If odd number of moves, add newline
  if (g_game_state.move_count % 2 == 1) {
    printf("\n");
  }
  printf("===================\n\n");
}

void print_last_moves(int count) {
  if (g_game_state.move_count <= 0) {
    printf("No moves made yet.\n");
    return;
  }

  int start = (g_game_state.move_count - count > 0)
                  ? g_game_state.move_count - count
                  : 0;

  printf("\n=== Last %d Moves ===\n", g_game_state.move_count - start);
  for (int i = start; i < g_game_state.move_count; i++) {
    move_history_t *move = &g_game_state.move_list[i];
    char notation[16];
    move_to_notation(move, notation, sizeof(notation));

    // Print move number and notation
    if (i % 2 == 0) {
      // White's move
      printf("%d. %s", (i / 2) + 1, notation);
    } else {
      // Black's move
      printf(" %s\n", notation);
    }
  }

  // If odd number of moves, add newline
  if ((g_game_state.move_count - start) % 2 == 1) {
    printf("\n");
  }
  printf("===================\n\n");
}

void print_help(void) {
  printf("\n=== Chess Game Controls ===\n");
  printf("Mouse:\n");
  printf("  Left Click - Select piece or make move\n");
  printf("\nKeyboard:\n");
  printf("  ESC - Exit game\n");
  printf("  SPACE - Pause/Resume game\n");
  printf("  H - Show full move history\n");
  printf("  L - Show last 10 moves\n");
  printf("  U - Undo last move\n");
  printf("  ? - Show this help\n");
  printf("===========================\n\n");
}

int undo_last_move(void) {
  if (g_game_state.move_count <= 0) {
    printf("No moves to undo.\n");
    return 0;
  }

  // Get the last move
  move_history_t *last_move =
      &g_game_state.move_list[g_game_state.move_count - 1];

  // Handle special moves
  if (last_move->is_castling) {
    // Undo castling - move king back and restore rook
    set_piece_at(&g_game_state.board, last_move->from_row, last_move->from_col,
                 last_move->moved_piece);

    // Restore rook position
    if (last_move->to_col > last_move->from_col) {
      // Kingside castling - restore rook from f to h
      piece_t rook = {ROOK, last_move->moved_piece.color,
                      last_move->moved_piece.theme};
      set_piece_at(&g_game_state.board, last_move->from_row, 7, rook);
      clear_piece_at(&g_game_state.board, last_move->to_row, last_move->to_col);
    } else {
      // Queenside castling - restore rook from d to a
      piece_t rook = {ROOK, last_move->moved_piece.color,
                      last_move->moved_piece.theme};
      set_piece_at(&g_game_state.board, last_move->from_row, 0, rook);
      clear_piece_at(&g_game_state.board, last_move->to_row, last_move->to_col);
    }
  } else if (last_move->is_en_passant) {
    // Undo en passant - restore pawn and captured pawn
    set_piece_at(&g_game_state.board, last_move->from_row, last_move->from_col,
                 last_move->moved_piece);

    // Restore captured pawn in the correct position
    int captured_row = (last_move->moved_piece.color == WHITE)
                           ? last_move->to_row + 1
                           : last_move->to_row - 1;
    set_piece_at(&g_game_state.board, captured_row, last_move->to_col,
                 last_move->captured_piece);
    clear_piece_at(&g_game_state.board, last_move->to_row, last_move->to_col);
  } else {
    // Regular move undo
    set_piece_at(&g_game_state.board, last_move->from_row, last_move->from_col,
                 last_move->moved_piece);

    // Restore captured piece if there was one
    if (last_move->captured_piece.type != EMPTY) {
      set_piece_at(&g_game_state.board, last_move->to_row, last_move->to_col,
                   last_move->captured_piece);
    } else {
      // Clear the destination square if no piece was captured
      clear_piece_at(&g_game_state.board, last_move->to_row, last_move->to_col);
    }
  }

  // Switch turn back
  g_game_state.current_turn =
      (g_game_state.current_turn == WHITE) ? BLACK : WHITE;
  g_game_state.move_count--;

  // Clear selection and possible moves
  g_game_state.selected_piece_row = -1;
  g_game_state.selected_piece_col = -1;
  clear_possible_moves();

  printf("Undid last move. It's now %s's turn.\n",
         (g_game_state.current_turn == WHITE) ? "White" : "Black");
  return 1;
}

void update_state(SDL_Renderer *renderer) {
  if (g_game_state.move_count == -1) {
    printf("Game started. White's turn.\n");
    g_game_state.move_count++;
  }

  if (g_game_state.input_state->mouse_clicked) {
    handle_mouse_click(renderer, g_game_state.input_state->mouse_x,
                       g_game_state.input_state->mouse_y);
    g_game_state.render_needed = 1; // Mark for re-render
  }

  if (g_game_state.input_state->print_history) {
    g_game_state.input_state->print_history = 0;
    print_history();
  }

  if (g_game_state.input_state->undo_move) {
    g_game_state.input_state->undo_move = 0;
    if (undo_last_move()) {
      g_game_state.render_needed = 1; // Mark for re-render
    }
  }

  if (g_game_state.input_state->show_last_moves) {
    g_game_state.input_state->show_last_moves = 0;
    print_last_moves(10); // Show last 10 moves
  }

  if (g_game_state.input_state->show_help) {
    g_game_state.input_state->show_help = 0;
    print_help();
  }
}
