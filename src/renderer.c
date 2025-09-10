#include "renderer.h"
#include "game.h"
#include "string.h"
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

void clear_screen(SDL_Surface *screen) {
  if (!screen)
    return;
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, COLOR_BLACK));
}

void draw_background(SDL_Surface *screen) {
  if (!screen)
    return;
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, COLOR_DARK_GRAY));
}

int get_cell_size(SDL_Surface *screen) {
  if (!screen)
    return 0;

  // Prevent non square boards by using the smallest side
  int smallestSide = (screen->w < screen->h) ? screen->w : screen->h;
  smallestSide -= (BOARD_OFFSET * 2); // Account for offset
  return smallestSide / BOARD_SIZE;
}

SDL_Rect get_square_rect(SDL_Surface *screen, int row, int col) {
  SDL_Rect rect = {0, 0, 0, 0};

  if (!screen || !is_valid_position(row, col)) {
    return rect;
  }

  int cell_size = get_cell_size(screen);
  rect.x = BOARD_OFFSET + col * cell_size;
  rect.y = BOARD_OFFSET + row * cell_size;
  rect.w = cell_size;
  rect.h = cell_size;

  return rect;
}

void draw_board(SDL_Surface *screen) {
  if (!screen)
    return;

  int cell_size = get_cell_size(screen);
  int board_size = cell_size * BOARD_SIZE;

  // Board outline, offset by BOARD_OFFSET pixels from each edge
  SDL_Rect board_rect = {BOARD_OFFSET, BOARD_OFFSET, board_size, board_size};
  SDL_FillRect(screen, &board_rect, SDL_MapRGB(screen->format, COLOR_BLACK));

  // Draw grid lines
  for (int i = 1; i < BOARD_SIZE; i++) {
    // Vertical lines
    SDL_Rect v_line = {board_rect.x + i * cell_size, board_rect.y,
                       BOARD_BORDER_WIDTH, board_rect.h};
    SDL_FillRect(screen, &v_line, SDL_MapRGB(screen->format, COLOR_WHITE));

    // Horizontal lines
    SDL_Rect h_line = {board_rect.x, board_rect.y + i * cell_size, board_rect.w,
                       BOARD_BORDER_WIDTH};
    SDL_FillRect(screen, &h_line, SDL_MapRGB(screen->format, COLOR_WHITE));
  }

  // Fill the squares with alternating colors
  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      SDL_Rect square = get_square_rect(screen, row, col);

      if ((row + col) % 2 == 1) { // Dark squares
        SDL_FillRect(screen, &square, SDL_MapRGB(screen->format, COLOR_BLACK));
      } else { // Light squares
        SDL_FillRect(screen, &square,
                     SDL_MapRGB(screen->format, COLOR_LIGHT_GRAY));
      }
    }
  }
}

void draw_piece(SDL_Surface *screen, int row, int col, struct piece_t *piece) {
  if (!screen || !piece || !is_valid_position(row, col)) {
    fprintf(stderr, "Invalid parameters to draw_piece\n");
    return;
  }

  if (piece->type == EMPTY || piece->color == NONE) {
    return; // Nothing to draw
  }

  // Get the appropriate sprite sheet from resource manager
  SDL_Surface *sprite_map =
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
  SDL_Rect square_rect = get_square_rect(screen, row, col);
  SDL_Rect dest_rect = {
      square_rect.x + (square_rect.w - PIECE_WIDTH * PIECE_SCALING) / 2,
      square_rect.y + (square_rect.h - PIECE_HEIGHT * PIECE_SCALING) / 2,
      PIECE_WIDTH * PIECE_SCALING, PIECE_HEIGHT * PIECE_SCALING};

  SDL_BlitScaled(sprite_map, &src_rect, screen, &dest_rect);
}

void draw_all_pieces(SDL_Surface *screen, board_t *board) {
  if (!screen || !board)
    return;

  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      struct piece_t *piece = get_piece_at(board, row, col);
      if (piece) {
        draw_piece(screen, row, col, piece);
      }
    }
  }
}

void draw_possible_moves(SDL_Surface *screen,
                         int possible_moves[BOARD_SIZE][BOARD_SIZE]) {
  if (!screen || !possible_moves)
    return;

  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      if (possible_moves[row][col]) {
        SDL_Rect square = get_square_rect(screen, row, col);
        // Draw a semi-transparent overlay for possible moves
        // For now, just draw a border
        SDL_Rect border = {square.x + 2, square.y + 2, square.w - 4,
                           square.h - 4};
        SDL_FillRect(screen, &border, SDL_MapRGB(screen->format, 0, 255, 0));
      }
    }
  }
}
