#include "engine.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// I don't like it being odd, but it's easier to contain
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 1280;

void drawCells(SDL_Surface *screen) {
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 128, 128, 128));
}

int main(void) {
  SDL_Window *win = NULL;
  SDL_Surface *screen = NULL;

  if (init(&win, &screen, SCREEN_WIDTH, SCREEN_HEIGHT) != 0) {
    return 1;
  }

  // manual malloc so we can use easy -> syntax
  struct InputState *currentState = malloc(sizeof(struct InputState));
  if (currentState == NULL) {
    fprintf(stderr, "Failed to allocate memory for InputState\n");
    return 1;
  }
  currentState->exitTrigger = 0;
  currentState->pause = 0;

  while (!currentState->exitTrigger) {
    handleEvents(currentState);

    SDL_UpdateWindowSurface(win);

    if (currentState->pause) {
      // Only run at 30 FPS when paused
      SDL_Delay(1000 / 30);
      continue;
    }

    // Run at 60 FPS when unpaused
    SDL_Delay(1000 / 60);
  }

  cleanup(win);
  return 0;
}
