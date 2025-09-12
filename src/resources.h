#ifndef RESOURCES_H
#define RESOURCES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "config.h"
#include "game.h"
#include "piece.h"

// Resource manager structure
typedef struct {
    SDL_Texture* white_default_sheet;
    SDL_Texture* black_default_sheet;
    SDL_Texture* white_wood_sheet;
    SDL_Texture* black_wood_sheet;
    int initialized;
} resource_manager_t;

// Global resource manager instance
extern resource_manager_t g_resources;

// Resource management functions
ErrorCode init_resources(SDL_Renderer* renderer);
void cleanup_resources(void);
SDL_Texture* get_piece_sprite(int piece_type, int color, int theme);
const char* get_sprite_filename(int color, int theme);

#endif // RESOURCES_H
