#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "game.h"
#include "config.h"
#include "resources.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>

// Rendering functions
ErrorCode init_renderer(void);
void cleanup_renderer(void);
void clearScreen(SDL_Surface *screen);
void drawBackground(SDL_Surface *screen);
void drawBoard(SDL_Surface *screen);
void drawPiece(SDL_Surface *screen, int row, int col, struct Piece* piece);
void drawPossibleMoves(SDL_Surface *screen, int possibleMoves[BOARD_SIZE][BOARD_SIZE]);
void drawAllPieces(SDL_Surface *screen, struct Board* board);

// Utility functions
int get_cell_size(SDL_Surface *screen);
SDL_Rect get_square_rect(SDL_Surface *screen, int row, int col);

#endif // RENDERER_H
