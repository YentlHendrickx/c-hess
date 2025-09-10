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

void render_game(SDL_Surface *screen, board_t *board) {
  // Clear screen
  clear_screen(screen);
  draw_background(screen);

  // Draw board
  draw_board(screen);

  // Draw pieces
  draw_all_pieces(screen, board);

  // // Test pieces with different themes
  // drawPiece(screen, 0, 0, &(struct Piece){ROOK, BLACK, THEME_DEFAULT});
  // drawPiece(screen, 0, 1, &(struct Piece){KNIGHT, BLACK, THEME_DEFAULT});
  // drawPiece(screen, 0, 2, &(struct Piece){BISHOP, BLACK, THEME_DEFAULT});
  // drawPiece(screen, 0, 3, &(struct Piece){QUEEN, BLACK, THEME_DEFAULT});
  // drawPiece(screen, 0, 4, &(struct Piece){KING, BLACK, THEME_DEFAULT});
  // drawPiece(screen, 0, 5, &(struct Piece){BISHOP, BLACK, THEME_DEFAULT});
  // drawPiece(screen, 0, 6, &(struct Piece){KNIGHT, BLACK, THEME_DEFAULT});
  // drawPiece(screen, 0, 7, &(struct Piece){ROOK, BLACK, THEME_DEFAULT});
  //
  // drawPiece(screen, 1, 0, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});
  // drawPiece(screen, 1, 1, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});
  // drawPiece(screen, 1, 2, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});
  // drawPiece(screen, 1, 3, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});
  // drawPiece(screen, 1, 4, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});
  // drawPiece(screen, 1, 5, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});
  // drawPiece(screen, 1, 6, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});
  // drawPiece(screen, 1, 7, &(struct Piece){PAWN, BLACK, THEME_DEFAULT});
  //
  // drawPiece(screen, 7, 0, &(struct Piece){ROOK, WHITE, THEME_DEFAULT});
  // drawPiece(screen, 7, 1, &(struct Piece){KNIGHT, WHITE, THEME_DEFAULT});
  // drawPiece(screen, 7, 2, &(struct Piece){BISHOP, WHITE, THEME_DEFAULT});
  // drawPiece(screen, 7, 3, &(struct Piece){QUEEN, WHITE, THEME_DEFAULT});
  // drawPiece(screen, 7, 4, &(struct Piece){KING, WHITE, THEME_DEFAULT});
  // drawPiece(screen, 7, 5, &(struct Piece){BISHOP, WHITE, THEME_DEFAULT});
  // drawPiece(screen, 7, 6, &(struct Piece){KNIGHT, WHITE, THEME_DEFAULT});
  // drawPiece(screen, 7, 7, &(struct Piece){ROOK, WHITE, THEME_DEFAULT});
  //
  // drawPiece(screen, 6, 0, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
  // drawPiece(screen, 6, 1, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
  // drawPiece(screen, 6, 2, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
  // drawPiece(screen, 6, 3, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
  // drawPiece(screen, 6, 4, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
  // drawPiece(screen, 6, 5, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
  // drawPiece(screen, 6, 6, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
  // drawPiece(screen, 6, 7, &(struct Piece){PAWN, WHITE, THEME_DEFAULT});
}

void game_loop(input_state_t *current_state, SDL_Surface *screen,
               game_state_t *state) {
  // 1. Handle input
  handle_events(current_state);

  // 2. Update game state
  int updates = update_state();

  static int first_run = 1;
  // 3. Render everything
  if (!updates && !first_run) {
    return;
  }

  render_game(screen, &state->board);
  first_run = 0;
}

int calculate_fps(Uint32 *last_frame_time, int *frame_count) {
  Uint32 current_frame_time = SDL_GetTicks();
  (*frame_count)++;

  if (current_frame_time - *last_frame_time >= FPS_UPDATE_INTERVAL) {
    int fps = *frame_count;
    *frame_count = 0;
    *last_frame_time = current_frame_time;
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

  game_state_t *state = init_game_state();

  // Allocate input state
  input_state_t *current_state = malloc(sizeof(input_state_t));
  if (current_state == NULL) {
    fprintf(stderr, "Failed to allocate memory for input_state_t\n");
    cleanup_game_state();
    cleanup_resources();
    cleanup_renderer();
    cleanup_engine(win);
    return 1;
  }
  init_input_state(current_state);

  // Calculate last frame time and FPS
  Uint32 last_frame_time = 0;
  int frame_count = 0;

  // Main game loop
  while (!current_state->exit_trigger) {
    int fps = calculate_fps(&last_frame_time, &frame_count);
    if (fps != -1) {
      char title[50];
      snprintf(title, sizeof(title), "C-Hess - FPS: %d", fps);
      SDL_SetWindowTitle(win, title);
    }

    game_loop(current_state, screen, state);
    SDL_UpdateWindowSurface(win);

    // Frame rate limiting
    if (current_state->pause) {
      SDL_Delay(1000 / PAUSED_FPS);
    } else {
      SDL_Delay(1000 / TARGET_FPS);
    }
  }

  // Cleanup
  free(current_state);
  cleanup_game_state();
  cleanup_resources();
  cleanup_renderer();
  cleanup_engine(win);

  return 0;
}
