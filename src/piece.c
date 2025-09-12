#include "piece.h"

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

void pawn_moves(piece_t piece, int start_row, int start_col, board_t *board,
                move_history_t move_list[1024], int move_count,
                int possible_moves[8][8]) {
  int direction = (piece.color == WHITE) ? -1 : 1; // White moves up, Black down
  int next_row = start_row + direction;

  // Move forward one square if empty
  if (is_valid_position(next_row, start_col)) {
    piece_t *front_piece = get_piece_at(board, next_row, start_col);
    if (front_piece && front_piece->type == EMPTY) {
      possible_moves[next_row][start_col] = 1;

      // If at starting position, can move two squares forward
      int starting_row = (piece.color == WHITE) ? 6 : 1;
      if (start_row == starting_row) {
        int two_ahead_row = start_row + 2 * direction;
        piece_t *two_ahead_piece =
            get_piece_at(board, two_ahead_row, start_col);
        if (two_ahead_piece && two_ahead_piece->type == EMPTY) {
          possible_moves[two_ahead_row][start_col] = 1;
        }
      }
    }
  }

  // Capture diagonally
  for (int col_offset = -1; col_offset <= 1; col_offset += 2) {
    int diag_col = start_col + col_offset;
    if (is_valid_position(next_row, diag_col)) {
      piece_t *diag_piece = get_piece_at(board, next_row, diag_col);
      if (diag_piece && diag_piece->type != EMPTY &&
          diag_piece->color != piece.color) {
        possible_moves[next_row][diag_col] = 1;
      }
    }
  }

  // En Passent based on history (not possible in less than 2 moves)
  if (move_count <= 2) {
    return;
  }

  move_history_t *last_move = &move_list[move_count - 1];
  if (last_move->moved_piece.type == PAWN &&
      abs(last_move->from_row - last_move->to_row) == 2 &&
      last_move->to_row == start_row) {
    // The opponent's pawn just moved two squares forward to the same row
    if (abs(last_move->to_col - start_col) == 1) {
      // The opponent's pawn is adjacent to our pawn
      possible_moves[start_row + direction][last_move->to_col] = 1;
    }
  }
}

void knight_moves(piece_t piece, int start_row, int start_col, board_t *board,
                  int possible_moves[8][8]) {
  int move_offsets[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
                            {1, -2},  {1, 2},  {2, -1},  {2, 1}};

  for (int i = 0; i < 8; i++) {
    int new_row = start_row + move_offsets[i][0];
    int new_col = start_col + move_offsets[i][1];

    if (is_valid_position(new_row, new_col)) {
      piece_t *target_piece = get_piece_at(board, new_row, new_col);
      if (target_piece->type == EMPTY || target_piece->color != piece.color) {
        possible_moves[new_row][new_col] = 1;
      }
    }
  }
}

void bishop_moves(piece_t piece, int start_row, int start_col, board_t *board,
                  int possible_moves[8][8]) {
  int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

  for (int d = 0; d < 4; d++) {
    int dr = directions[d][0];
    int dc = directions[d][1];
    int r = start_row + dr;
    int c = start_col + dc;

    while (is_valid_position(r, c)) {
      piece_t *target_piece = get_piece_at(board, r, c);
      if (target_piece->type == EMPTY) {
        possible_moves[r][c] = 1;
      } else {
        if (target_piece->color != piece.color) {
          possible_moves[r][c] = 1;
        }
        break; // Blocked by another piece
      }
      r += dr;
      c += dc;
    }
  }
}

void rook_moves(piece_t piece, int start_row, int start_col, board_t *board,
                int possible_moves[8][8]) {
  int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  for (int d = 0; d < 4; d++) {
    int dr = directions[d][0];
    int dc = directions[d][1];
    int r = start_row + dr;
    int c = start_col + dc;

    while (is_valid_position(r, c)) {
      piece_t *target_piece = get_piece_at(board, r, c);
      if (target_piece->type == EMPTY) {
        possible_moves[r][c] = 1;
      } else {
        if (target_piece->color != piece.color) {
          possible_moves[r][c] = 1;
        }
        break; // Blocked by another piece
      }
      r += dr;
      c += dc;
    }
  }
}

void queen_moves(piece_t piece, int start_row, int start_col, board_t *board,
                 int possible_moves[8][8]) {
  // Combine rook and bishop moves
  rook_moves(piece, start_row, start_col, board, possible_moves);
  bishop_moves(piece, start_row, start_col, board, possible_moves);
}

// Helper function to check if a square is under attack
int is_square_under_attack(board_t *board, int row, int col,
                           int attacking_color) {
  // Check all pieces of the attacking color
  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      piece_t *piece = get_piece_at(board, r, c);
      if (!(piece && piece->type != EMPTY && piece->color == attacking_color)) {
        continue; // Not an attacking piece
      }

      // Check if this piece can attack the target square
      int temp_moves[8][8] = {0};

      if (piece->type == PAWN) {
        // Pawns attack diagonally
        int direction = (piece->color == WHITE) ? -1 : 1;
        int attack_row = r + direction;
        for (int col_offset = -1; col_offset <= 1; col_offset += 2) {
          int attack_col = c + col_offset;
          if (is_valid_position(attack_row, attack_col) && attack_row == row &&
              attack_col == col) {
            return 1;
          }
        }
      } else if (piece->type == KNIGHT) {
        knight_moves(*piece, r, c, board, temp_moves);
        if (temp_moves[row][col])
          return 1;
      } else if (piece->type == BISHOP) {
        bishop_moves(*piece, r, c, board, temp_moves);
        if (temp_moves[row][col])
          return 1;
      } else if (piece->type == ROOK) {
        rook_moves(*piece, r, c, board, temp_moves);
        if (temp_moves[row][col])
          return 1;
      } else if (piece->type == QUEEN) {
        queen_moves(*piece, r, c, board, temp_moves);
        if (temp_moves[row][col])
          return 1;
      } else if (piece->type == KING) {
        // King can attack adjacent squares
        int move_offsets[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                                  {0, 1},   {1, -1}, {1, 0},  {1, 1}};
        for (int i = 0; i < 8; i++) {
          int new_row = r + move_offsets[i][0];
          int new_col = c + move_offsets[i][1];
          if (new_row == row && new_col == col) {
            return 1;
          }
        }
      }
    }
  }
  return 0;
}

void king_moves(piece_t piece, int start_row, int start_col, board_t *board,
                int possible_moves[8][8], int can_castle_kingside,
                int can_castle_queenside) {
  int move_offsets[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                            {0, 1},   {1, -1}, {1, 0},  {1, 1}};

  for (int i = 0; i < 8; i++) {
    int new_row = start_row + move_offsets[i][0];
    int new_col = start_col + move_offsets[i][1];

    if (is_valid_position(new_row, new_col)) {
      piece_t *target_piece = get_piece_at(board, new_row, new_col);
      if (target_piece->type == EMPTY || target_piece->color != piece.color) {
        possible_moves[new_row][new_col] = 1;
      }
    }
  }

  // Castling. We can only castle if we have the right and have a clean line of
  // sight
  int starting_row = (piece.color == WHITE) ? 7 : 0;
  int king_initial_col = 4;

  // Figure out if we are castling kingside or queenside
  if (start_row == starting_row && start_col == king_initial_col) {

    // Check if king is currently in check
    int opponent_color = (piece.color == WHITE) ? BLACK : WHITE;
    if (is_square_under_attack(board, start_row, start_col, opponent_color)) {
      return; // Can't castle while in check
    }

    // Kingside castling
    if (can_castle_kingside) {
      // Check squares between king and rook are empty
      if (board->squares[starting_row][5].piece.type == EMPTY &&
          board->squares[starting_row][6].piece.type == EMPTY) {
        // Check if king would pass through check
        if (!is_square_under_attack(board, starting_row, 5, opponent_color) &&
            !is_square_under_attack(board, starting_row, 6, opponent_color)) {
          possible_moves[starting_row][6] = 1; // Castling move
        }
      }
    }

    // Queenside castling
    if (can_castle_queenside) {
      // Check squares between king and rook are empty
      if (board->squares[starting_row][1].piece.type == EMPTY &&
          board->squares[starting_row][2].piece.type == EMPTY &&
          board->squares[starting_row][3].piece.type == EMPTY) {
        // Check if king would pass through check
        if (!is_square_under_attack(board, starting_row, 2, opponent_color) &&
            !is_square_under_attack(board, starting_row, 3, opponent_color)) {
          possible_moves[starting_row][2] = 1; // Castling move
        }
      }
    }
  }
}

void get_allowed_moves(game_state_t *game_state, int start_row, int start_col) {
  piece_t piece = game_state->board.squares[start_row][start_col].piece;
  board_t *board = &game_state->board;
  move_history_t *move_list = game_state->move_list;
  int move_count = game_state->move_count;
  int (*possible_moves)[8] = game_state->possible_moves;
  int temp[8][8] = {0};

  if (piece.type == PAWN) {
    pawn_moves(piece, start_row, start_col, board, move_list, move_count, temp);
  } else if (piece.type == KNIGHT) {
    knight_moves(piece, start_row, start_col, board, temp);
  } else if (piece.type == BISHOP) {
    bishop_moves(piece, start_row, start_col, board, temp);
  } else if (piece.type == ROOK) {
    rook_moves(piece, start_row, start_col, board, temp);
  } else if (piece.type == QUEEN) {
    queen_moves(piece, start_row, start_col, board, temp);
  } else if (piece.type == KING) {
    // Get castling rights for the current player
    int can_castle_kingside, can_castle_queenside;
    if (piece.color == WHITE) {
      can_castle_kingside = game_state->white_can_castle_kingside;
      can_castle_queenside = game_state->white_can_castle_queenside;
    } else {
      can_castle_kingside = game_state->black_can_castle_kingside;
      can_castle_queenside = game_state->black_can_castle_queenside;
    }
    king_moves(piece, start_row, start_col, board, temp, can_castle_kingside,
               can_castle_queenside);
  } else {
    // Invalid piece type
    fprintf(stderr, "Invalid piece type %d for allowed moves\n", piece.type);
    return;
  }

  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      possible_moves[r][c] = temp[r][c];
    }
  }
}

/// Validation functions
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

/// Board manipulation functions
piece_t *get_piece_at(board_t *board, int row, int col) {
  if (!board || !is_valid_position(row, col)) {
    return NULL;
  }
  return &board->squares[row][col].piece;
}

int set_piece_at(board_t *board, int row, int col, piece_t piece) {
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
