#include "renderer.h"
#include "game.h"
#include <stdio.h>

ErrorCode init_renderer(void) {
  // Initialize SDL_image
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n",
            IMG_GetError());
    return ERROR_FILE_LOAD;
  }
  return ERROR_NONE;
}

void cleanup_renderer(void) { IMG_Quit(); }

void clear_screen(SDL_Renderer *renderer) {
  if (!renderer)
    return;
  SDL_SetRenderDrawColor(renderer, COLOR_BLACK, 255);
  SDL_RenderClear(renderer);
}

void draw_background(SDL_Renderer *renderer) {
  if (!renderer)
    return;
  SDL_SetRenderDrawColor(renderer, COLOR_DARK_GRAY, 255);
  SDL_RenderClear(renderer);
}

int get_cell_size(SDL_Renderer *renderer) {
  if (!renderer)
    return 0;

  // Get renderer output size
  int w, h;
  SDL_GetRendererOutputSize(renderer, &w, &h);

  // Prevent non square boards by using the smallest side
  int smallestSide = (w < h) ? w : h;
  smallestSide -= (BOARD_OFFSET * 2); // Account for offset
  return smallestSide / BOARD_SIZE;
}

SDL_Rect get_square_rect(SDL_Renderer *renderer, int row, int col) {
  SDL_Rect rect = {0, 0, 0, 0};

  if (!renderer || !is_valid_position(row, col)) {
    return rect;
  }

  int cell_size = get_cell_size(renderer);
  rect.x = BOARD_OFFSET + col * cell_size;
  rect.y = BOARD_OFFSET + row * cell_size;
  rect.w = cell_size;
  rect.h = cell_size;

  return rect;
}

void draw_board(SDL_Renderer *renderer) {
  if (!renderer)
    return;

  int cell_size = get_cell_size(renderer);
  int board_size = cell_size * BOARD_SIZE;

  // Board outline, offset by BOARD_OFFSET pixels from each edge
  SDL_Rect board_rect = {BOARD_OFFSET, BOARD_OFFSET, board_size, board_size};
  SDL_SetRenderDrawColor(renderer, COLOR_BLACK, 255);
  SDL_RenderFillRect(renderer, &board_rect);

  // Draw grid lines
  SDL_SetRenderDrawColor(renderer, COLOR_WHITE, 255);
  for (int i = 1; i < BOARD_SIZE; i++) {
    // Vertical lines
    SDL_Rect v_line = {board_rect.x + i * cell_size, board_rect.y,
                       BOARD_BORDER_WIDTH, board_rect.h};
    SDL_RenderFillRect(renderer, &v_line);

    // Horizontal lines
    SDL_Rect h_line = {board_rect.x, board_rect.y + i * cell_size, board_rect.w,
                       BOARD_BORDER_WIDTH};
    SDL_RenderFillRect(renderer, &h_line);
  }

  // Fill the squares with alternating colors
  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      SDL_Rect square = get_square_rect(renderer, row, col);

      if ((row + col) % 2 == 1) { // Dark squares
        SDL_SetRenderDrawColor(renderer, COLOR_BLACK, 255);
      } else { // Light squares
        SDL_SetRenderDrawColor(renderer, COLOR_LIGHT_GRAY, 255);
      }
      SDL_RenderFillRect(renderer, &square);
    }
  }
}

void draw_piece(SDL_Renderer *renderer, int row, int col, piece_t *piece) {
  if (!renderer || !piece || !is_valid_position(row, col)) {
    fprintf(stderr, "Invalid parameters to draw_piece\n");
    return;
  }

  if (piece->type == EMPTY || piece->color == NONE) {
    return; // Nothing to draw
  }

  // Get the appropriate sprite sheet from resource manager
  SDL_Texture *sprite_map =
      get_piece_sprite(piece->type, piece->color, piece->theme);
  if (!sprite_map) {
    fprintf(stderr,
            "Failed to get sprite for piece type %d, color %d, theme %d\n",
            piece->type, piece->color, piece->theme);
    return;
  }

  // Calculate source rectangle
  int src_x = PIECE_WIDTH * (piece->type - 1);
  SDL_Rect src_rect = {src_x, 0, PIECE_WIDTH, PIECE_HEIGHT};

  // Calculate destination rectangle
  SDL_Rect square_rect = get_square_rect(renderer, row, col);
  SDL_Rect dest_rect = {
      square_rect.x + (square_rect.w - PIECE_WIDTH * PIECE_SCALING) / 2,
      square_rect.y + (square_rect.h - PIECE_HEIGHT * PIECE_SCALING) / 2,
      PIECE_WIDTH * PIECE_SCALING, PIECE_HEIGHT * PIECE_SCALING};

  SDL_RenderCopy(renderer, sprite_map, &src_rect, &dest_rect);
}

void draw_all_pieces(SDL_Renderer *renderer, board_t *board) {
  if (!renderer || !board)
    return;

  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      piece_t *piece = get_piece_at(board, row, col);
      if (piece) {
        draw_piece(renderer, row, col, piece);
      }
    }
  }
}

void draw_possible_moves(SDL_Renderer *renderer,
                         int possible_moves[BOARD_SIZE][BOARD_SIZE],
                         board_t *board) {
  if (!renderer || !possible_moves)
    return;

  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      if (possible_moves[row][col]) {
        SDL_Rect square = get_square_rect(renderer, row, col);
        // Draw a semi-transparent overlay for possible moves
        // For now, just draw a border
        SDL_Rect border = {square.x + 2, square.y + 2, square.w - 4,
                           square.h - 4};

        if (board->squares[row][col].piece.type != EMPTY) {
          // Capture move - red border
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 50);
        } else {
          // Regular move - green border
          SDL_SetRenderDrawColor(renderer, 0, 255, 0, 50);
        }

        SDL_RenderFillRect(renderer, &border);
      }
    }
  }
}
