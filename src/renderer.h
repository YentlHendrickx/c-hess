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
void clear_screen(SDL_Surface *screen);
void draw_background(SDL_Surface *screen);
void draw_board(SDL_Surface *screen);
void draw_piece(SDL_Surface *screen, int row, int col, piece_t* piece);
void draw_possible_moves(SDL_Surface *screen, int possible_moves[BOARD_SIZE][BOARD_SIZE]);
void draw_all_pieces(SDL_Surface *screen, board_t* board);

// Utility functions
int get_cell_size(SDL_Surface *screen);
SDL_Rect get_square_rect(SDL_Surface *screen, int row, int col);

#endif // RENDERER_H
