#include "engine.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

ErrorCode init_engine(SDL_Window **win, SDL_Renderer **renderer, int width,
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

  *renderer = SDL_CreateRenderer(
      *win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (*renderer == NULL) {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(*win);
    SDL_Quit();
    return ERROR_SURFACE_CREATE;
  }

  // Set renderer properties
  SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);

  return ERROR_NONE;
}

void cleanup_engine(SDL_Window *win, SDL_Renderer *renderer) {
  if (renderer != NULL) {
    SDL_DestroyRenderer(renderer);
  }
  if (win != NULL) {
    SDL_DestroyWindow(win);
  }
  SDL_Quit();
}
