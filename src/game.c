#include "game.h"
#include <SDL2/SDL_ttf.h>

void fillBackground(SDL_Surface *screen) {
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 180, 180, 180));
}

// TODO: move the drawing of everything into it's own file (render.c maybe?)
void drawBoard(SDL_Surface *screen) {
  fillBackground(screen);

  // Prevent non square boards by using the smallest side
  int smallestSide = (screen->w < screen->h) ? screen->w : screen->h;
  smallestSide -= 40; // Account for offset
  int cellSize = smallestSide / 8;

  // Board outline, offset by 20 pixels from each edge
  SDL_Rect boardRect = {20, 20, smallestSide, smallestSide};
  SDL_FillRect(screen, &boardRect, SDL_MapRGB(screen->format, 0, 0, 0));

  for (int i = 1; i < 8; i++) {
    // Vertical lines
    SDL_Rect vLine = {boardRect.x + i * cellSize, boardRect.y, 2, boardRect.h};
    SDL_FillRect(screen, &vLine, SDL_MapRGB(screen->format, 255, 255, 255));

    // Horizontal lines
    SDL_Rect hLine = {boardRect.x, boardRect.y + i * cellSize, boardRect.w, 2};
    SDL_FillRect(screen, &hLine, SDL_MapRGB(screen->format, 255, 255, 255));
  }

  // Next we fill the squares
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if ((row + col) % 2 == 1) { // Dark squares
        SDL_Rect square = {boardRect.x + col * cellSize,
                           boardRect.y + row * cellSize, cellSize, cellSize};
        SDL_FillRect(screen, &square, SDL_MapRGB(screen->format, 0, 0, 0));
        continue;
      }

      SDL_Rect square = {boardRect.x + col * cellSize,
                         boardRect.y + row * cellSize, cellSize, cellSize};
      SDL_FillRect(screen, &square, SDL_MapRGB(screen->format, 240, 240, 240));
    }
  }
}

void drawGame(SDL_Surface *screen) { drawBoard(screen); }
