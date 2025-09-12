#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include "config.h"
#include "input.h"

// Engine functions
ErrorCode init_engine(SDL_Window **win, SDL_Renderer **renderer, int width, int height);
void cleanup_engine(SDL_Window *win, SDL_Renderer *renderer);

#endif // ENGINE_H
