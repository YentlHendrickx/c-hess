#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "game.h"
#include "config.h"
#include "resources.h"
#include <SDL2/SDL_image.h>

// Rendering functions
ErrorCode init_renderer(void);
void cleanup_renderer(void);
void clear_screen(SDL_Renderer *renderer);
void draw_background(SDL_Renderer *renderer);
void draw_board(SDL_Renderer *renderer);
void draw_piece(SDL_Renderer *renderer, int row, int col, piece_t* piece);
void draw_possible_moves(SDL_Renderer *renderer, int possible_moves[BOARD_SIZE][BOARD_SIZE], board_t* board);
void draw_all_pieces(SDL_Renderer *renderer, board_t* board);

// Utility functions
int get_cell_size(SDL_Renderer *renderer);
SDL_Rect get_square_rect(SDL_Renderer *renderer, int row, int col);

#endif // RENDERER_H
