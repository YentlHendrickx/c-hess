#include "engine.h"
#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 1280;

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

  // Calculate last frame time and FPS
  int fps = 0;
  Uint32 lastFrameTime = 0;
  Uint32 currentFrameTime = 0;

  while (!currentState->exitTrigger) {
    currentFrameTime = SDL_GetTicks();
    if (currentFrameTime - lastFrameTime >= 1000) {
      // Display fps in the window title
      char title[50];
      snprintf(title, sizeof(title), "Game - FPS: %d", fps);
      SDL_SetWindowTitle(win, title);

      fps = 0;
      lastFrameTime = currentFrameTime;
    }
    fps++;

    handleEvents(currentState);
    drawGame(screen);

    SDL_UpdateWindowSurface(win);

    if (currentState->pause) {
      // run at 30 fps when paused
      SDL_Delay(1000 / 30);
      continue;
    }

    SDL_Delay(1000 / 60);
  }

  cleanup(win);
  return 0;
}
