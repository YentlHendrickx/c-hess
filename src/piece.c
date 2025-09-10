#include "piece.h"

void pawn_moves(piece_t piece, int start_row, int start_col, board_t *board,
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

void king_moves(piece_t piece, int start_row, int start_col, board_t *board,
                int possible_moves[8][8]) {
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
}

void get_allowed_moves(piece_t piece, int start_row, int start_col,
                       board_t *board,
                       int possible_moves[BOARD_SIZE][BOARD_SIZE]) {

  int temp[BOARD_SIZE][BOARD_SIZE] = {0};

  if (piece.type == PAWN) {
    pawn_moves(piece, start_row, start_col, board, temp);
  } else if (piece.type == KNIGHT) {
    knight_moves(piece, start_row, start_col, board, temp);
  } else if (piece.type == BISHOP) {
    bishop_moves(piece, start_row, start_col, board, temp);
  } else if (piece.type == ROOK) {
    rook_moves(piece, start_row, start_col, board, temp);
  } else if (piece.type == QUEEN) {
    queen_moves(piece, start_row, start_col, board, temp);
  } else if (piece.type == KING) {
    king_moves(piece, start_row, start_col, board, temp);
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
