#include "engine.h"
#include <stdio.h>

int init(SDL_Window **win, SDL_Surface **screen, int width, int height) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  *win =
      SDL_CreateWindow("C-Hess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_SHOWN);

  if (win == NULL) {
    fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
    return 1;
  }

  *screen = SDL_GetWindowSurface(*win);
  if (*screen == NULL) {
    fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(*win);
    return 1;
  }

  return 0;
}

void cleanup(SDL_Window *win) {
  if (win != NULL) {
    SDL_DestroyWindow(win);
  }

  SDL_Quit();
}

void handleEvents(struct InputState *state) {
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      state->exitTrigger = 1;
      printf("Quit event detected\n");
      continue;
    }

    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
      case SDLK_ESCAPE:
        printf("Escape key pressed, exiting\n");
        state->exitTrigger = 1;
        continue;
      case SDLK_SPACE:
        state->pause = !state->pause;
        printf("Space key pressed, toggling pause to %d\n", state->pause);
        continue;
      }
    }
  }
}
