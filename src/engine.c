#include "engine.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

int init(SDL_Window **win, SDL_Surface **screen, int width, int height) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  *win =
      SDL_CreateWindow("C-Hess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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

void handleQuitEvent(struct InputState *state) {
  state->exitTrigger = 1;
  printf("Quit event detected\n");
}

void handleKeyDownEvent(struct InputState *state, SDL_Keysym keysym) {
  switch (keysym.sym) {
  case SDLK_ESCAPE:
    printf("Escape key pressed, exiting\n");
    state->exitTrigger = 1;
    break;
  case SDLK_SPACE:
    state->pause = !state->pause;
    printf("Space key pressed, toggling pause to %d\n", state->pause);
    break;
  default:
    break;
  }
}

void handleEvents(struct InputState *state) {
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      handleQuitEvent(state);
      break;
    case SDL_KEYDOWN:
      handleKeyDownEvent(state, e.key.keysym);
      break;
    }
  }
}
