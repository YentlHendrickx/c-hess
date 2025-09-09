#ifndef RESOURCES_H
#define RESOURCES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "config.h"
#include "game.h"

// Resource manager structure
typedef struct {
    SDL_Surface* white_default_sheet;
    SDL_Surface* black_default_sheet;
    SDL_Surface* white_wood_sheet;
    SDL_Surface* black_wood_sheet;
    int initialized;
} ResourceManager;

// Global resource manager instance
extern ResourceManager g_resources;

// Resource management functions
ErrorCode init_resources(void);
void cleanup_resources(void);
SDL_Surface* get_piece_sprite(int piece_type, int color, int theme);
const char* get_sprite_filename(int color, int theme);

#endif // RESOURCES_H
