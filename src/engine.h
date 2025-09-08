#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>

struct InputState {
  int exitTrigger;
  int pause;
};

int init(SDL_Window **win, SDL_Surface **screen, int width, int height);
void cleanup(SDL_Window *win);
void handleEvents(struct InputState *state);

#endif // ENGINE_H
