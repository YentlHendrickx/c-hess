#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include "config.h"
#include "input.h"

// Engine functions
ErrorCode init_engine(SDL_Window **win, SDL_Surface **screen, int width, int height);
void cleanup_engine(SDL_Window *win);

// Legacy functions for backward compatibility
int init(SDL_Window **win, SDL_Surface **screen, int width, int height);
void cleanup(SDL_Window *win);
void handleEvents(InputState *state);

#endif // ENGINE_H
