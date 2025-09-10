#include "config.h"
#include "engine.h"
#include "game.h"
#include "input.h"
#include "renderer.h"
#include "resources.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void renderGame(SDL_Surface *screen) {
  // Clear screen
  clearScreen(screen);
  drawBackground(screen);

  // Draw board
  drawBoard(screen);

  // Test pieces with different themes
  drawPiece(screen, 0, 0, &(struct Piece){ROOK, BLACK, THEME_DEFAULT});
  drawPiece(screen, 0, 1, &(struct Piece){KNIGHT, BLACK, THEME_DEFAULT});
  drawPiece(screen, 0, 2, &(struct Piece){BISHOP, BLACK, THEME_DEFAULT});
  drawPiece(screen, 0, 3, &(struct Piece){QUEEN, BLACK, THEME_DEFAULT});
  drawPiece(screen, 0, 4, &(struct Piece){KING, BLACK, THEME_DEFAULT});
  drawPiece(screen, 0, 5, &(struct Piece){BISHOP, BLACK, THEME_DEFAULT});
  drawPiece(screen, 0, 6, &(struct Piece){KNIGHT, BLACK, THEME_DEFAULT});
  drawPiece(screen, 0, 7, &(struct Piece){ROOK, BLACK, THEME_DEFAULT});

  drawPiece(screen, 1, 0, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});
  drawPiece(screen, 1, 1, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});
  drawPiece(screen, 1, 2, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});
  drawPiece(screen, 1, 3, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});
  drawPiece(screen, 1, 4, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});
  drawPiece(screen, 1, 5, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});
  drawPiece(screen, 1, 6, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});
  drawPiece(screen, 1, 7, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});

  drawPiece(screen, 7, 0, &(struct Piece){ROOK, WHITE, THEME_DEFAULT});
  drawPiece(screen, 7, 1, &(struct Piece){KNIGHT, WHITE, THEME_DEFAULT});
  drawPiece(screen, 7, 2, &(struct Piece){BISHOP, WHITE, THEME_DEFAULT});
  drawPiece(screen, 7, 3, &(struct Piece){QUEEN, WHITE, THEME_DEFAULT});
  drawPiece(screen, 7, 4, &(struct Piece){KING, WHITE, THEME_DEFAULT});
  drawPiece(screen, 7, 5, &(struct Piece){BISHOP, WHITE, THEME_DEFAULT});
  drawPiece(screen, 7, 6, &(struct Piece){KNIGHT, WHITE, THEME_DEFAULT});
  drawPiece(screen, 7, 7, &(struct Piece){ROOK, WHITE, THEME_DEFAULT});

  drawPiece(screen, 6, 0, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
  drawPiece(screen, 6, 1, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
  drawPiece(screen, 6, 2, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
  drawPiece(screen, 6, 3, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
  drawPiece(screen, 6, 4, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
  drawPiece(screen, 6, 5, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
  drawPiece(screen, 6, 6, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
  drawPiece(screen, 6, 7, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
}

void gameLoop(InputState *currentState, SDL_Surface *screen) {
  // 1. Handle input
  handle_events(currentState);

  // 2. Update game state
  int updates = updateState();

  static int firstRun = 1;
  // 3. Render everything
  if (!updates && !firstRun) {
    return;
  }

  renderGame(screen);
  firstRun = 0;
}

int calculateFPS(Uint32 *lastFrameTime, int *frameCount) {
  Uint32 currentFrameTime = SDL_GetTicks();
  (*frameCount)++;

  if (currentFrameTime - *lastFrameTime >= FPS_UPDATE_INTERVAL) {
    int fps = *frameCount;
    *frameCount = 0;
    *lastFrameTime = currentFrameTime;
    return fps;
  }

  return -1; // Not yet time to update FPS
}

int main(void) {
  SDL_Window *win = NULL;
  SDL_Surface *screen = NULL;
  ErrorCode result;

  // Initialize all systems
  result = init_engine(&win, &screen, SCREEN_WIDTH, SCREEN_HEIGHT);
  if (result != ERROR_NONE) {
    fprintf(stderr, "Failed to initialize engine: %d\n", result);
    return 1;
  }

  result = init_renderer();
  if (result != ERROR_NONE) {
    fprintf(stderr, "Failed to initialize renderer: %d\n", result);
    cleanup_engine(win);
    return 1;
  }

  result = init_resources();
  if (result != ERROR_NONE) {
    fprintf(stderr, "Failed to initialize resources: %d\n", result);
    cleanup_renderer();
    cleanup_engine(win);
    return 1;
  }

  result = init_game_state();
  if (result != ERROR_NONE) {
    fprintf(stderr, "Failed to initialize game state: %d\n", result);
    cleanup_resources();
    cleanup_renderer();
    cleanup_engine(win);
    return 1;
  }

  // Allocate input state
  InputState *currentState = malloc(sizeof(InputState));
  if (currentState == NULL) {
    fprintf(stderr, "Failed to allocate memory for InputState\n");
    cleanup_game_state();
    cleanup_resources();
    cleanup_renderer();
    cleanup_engine(win);
    return 1;
  }
  init_input_state(currentState);

  // Calculate last frame time and FPS
  Uint32 lastFrameTime = 0;
  int frameCount = 0;

  // Main game loop
  while (!currentState->exit_trigger) {
    int fps = calculateFPS(&lastFrameTime, &frameCount);
    if (fps != -1) {
      char title[50];
      snprintf(title, sizeof(title), "C-Hess - FPS: %d", fps);
      SDL_SetWindowTitle(win, title);
    }

    gameLoop(currentState, screen);
    SDL_UpdateWindowSurface(win);

    // Frame rate limiting
    if (currentState->pause) {
      SDL_Delay(1000 / PAUSED_FPS);
    } else {
      SDL_Delay(1000 / TARGET_FPS);
    }
  }

  // Cleanup
  free(currentState);
  cleanup_game_state();
  cleanup_resources();
  cleanup_renderer();
  cleanup_engine(win);

  return 0;
}
