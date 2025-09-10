#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include "config.h"
#include "input.h"

// Engine functions
ErrorCode init_engine(SDL_Window **win, SDL_Surface **screen, int width, int height);
void cleanup_engine(SDL_Window *win);

#endif // ENGINE_H
