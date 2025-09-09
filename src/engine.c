#include "engine.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

ErrorCode init_engine(SDL_Window **win, SDL_Surface **screen, int width,
                      int height) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return ERROR_SDL_INIT;
  }

  *win =
      SDL_CreateWindow("C-Hess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (*win == NULL) {
    fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return ERROR_WINDOW_CREATE;
  }

  *screen = SDL_GetWindowSurface(*win);
  if (*screen == NULL) {
    fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(*win);
    SDL_Quit();
    return ERROR_SURFACE_CREATE;
  }

  return ERROR_NONE;
}

void cleanup_engine(SDL_Window *win) {
  if (win != NULL) {
    SDL_DestroyWindow(win);
  }
  SDL_Quit();
}

// Legacy functions for backward compatibility
int init(SDL_Window **win, SDL_Surface **screen, int width, int height) {
  ErrorCode result = init_engine(win, screen, width, height);
  return (result == ERROR_NONE) ? 0 : 1;
}

void cleanup(SDL_Window *win) { cleanup_engine(win); }

void handleEvents(InputState *state) {
  // This function is now just a wrapper around the input module
  // The actual implementation is in input.c
  // This maintains backward compatibility
  if (state) {
    handle_events(state);
  }
}
