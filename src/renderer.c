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

void clearScreen(SDL_Surface *screen) {
  if (!screen)
    return;
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, COLOR_BLACK));
}

void drawBackground(SDL_Surface *screen) {
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

  int cellSize = get_cell_size(screen);
  rect.x = BOARD_OFFSET + col * cellSize;
  rect.y = BOARD_OFFSET + row * cellSize;
  rect.w = cellSize;
  rect.h = cellSize;

  return rect;
}

void drawBoard(SDL_Surface *screen) {
  if (!screen)
    return;

  int cellSize = get_cell_size(screen);
  int boardSize = cellSize * BOARD_SIZE;

  // Board outline, offset by BOARD_OFFSET pixels from each edge
  SDL_Rect boardRect = {BOARD_OFFSET, BOARD_OFFSET, boardSize, boardSize};
  SDL_FillRect(screen, &boardRect, SDL_MapRGB(screen->format, COLOR_BLACK));

  // Draw grid lines
  for (int i = 1; i < BOARD_SIZE; i++) {
    // Vertical lines
    SDL_Rect vLine = {boardRect.x + i * cellSize, boardRect.y,
                      BOARD_BORDER_WIDTH, boardRect.h};
    SDL_FillRect(screen, &vLine, SDL_MapRGB(screen->format, COLOR_WHITE));

    // Horizontal lines
    SDL_Rect hLine = {boardRect.x, boardRect.y + i * cellSize, boardRect.w,
                      BOARD_BORDER_WIDTH};
    SDL_FillRect(screen, &hLine, SDL_MapRGB(screen->format, COLOR_WHITE));
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

void drawPiece(SDL_Surface *screen, int row, int col, struct Piece *piece) {
  if (!screen || !piece || !is_valid_position(row, col)) {
    fprintf(stderr, "Invalid parameters to drawPiece\n");
    return;
  }

  if (piece->type == EMPTY || piece->color == NONE) {
    fprintf(stderr, "No piece to draw at (%d, %d)\n", row, col);
    return; // Nothing to draw
  }

  // Get the appropriate sprite sheet from resource manager
  SDL_Surface *spriteMap =
      get_piece_sprite(piece->type, piece->color, piece->theme);
  if (!spriteMap) {
    fprintf(stderr,
            "Failed to get sprite for piece type %d, color %d, theme %d\n",
            piece->type, piece->color, piece->theme);
    return;
  }

  // Calculate source rectangle
  int srcX = PIECE_WIDTH * (piece->type - 1);
  SDL_Rect srcRect = {srcX, 0, PIECE_WIDTH, PIECE_HEIGHT};

  // Calculate destination rectangle
  SDL_Rect squareRect = get_square_rect(screen, row, col);
  SDL_Rect destRect = {
      squareRect.x + (squareRect.w - PIECE_WIDTH * PIECE_SCALING) / 2,
      squareRect.y + (squareRect.h - PIECE_HEIGHT * PIECE_SCALING) / 2,
      PIECE_WIDTH * PIECE_SCALING, PIECE_HEIGHT * PIECE_SCALING};

  printf("Drawing piece type %d at (%d, %d) to screen at (%d, %d) with size "
         "(%d, %d)\n",
         piece->type, row, col, destRect.x, destRect.y, destRect.w, destRect.h);
  SDL_BlitScaled(spriteMap, &srcRect, screen, &destRect);
}

void drawAllPieces(SDL_Surface *screen, struct Board *board) {
  if (!screen || !board)
    return;

  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      struct Piece *piece = get_piece_at(board, row, col);
      if (piece) {
        drawPiece(screen, row, col, piece);
      }
    }
  }
}

void drawPossibleMoves(SDL_Surface *screen,
                       int possibleMoves[BOARD_SIZE][BOARD_SIZE]) {
  if (!screen || !possibleMoves)
    return;

  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      if (possibleMoves[row][col]) {
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
