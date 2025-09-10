#include "input.h"
#include <stdio.h>
#include <string.h>

void init_input_state(input_state_t *state) {
  if (!state)
    return;

  state->exit_trigger = 0;
  state->pause = 0;
  state->mouse_x = 0;
  state->mouse_y = 0;
  state->mouse_clicked = 0;
  memset(state->key_pressed, 0, sizeof(state->key_pressed));
}

void handle_quit_event(input_state_t *state) {
  if (!state)
    return;

  state->exit_trigger = 1;
  printf("Quit event detected\n");
}

void handle_keydown_event(input_state_t *state, SDL_Keysym keysym) {
  if (!state)
    return;

  // Mark key as pressed
  if (keysym.sym < 256) {
    state->key_pressed[keysym.sym] = 1;
  }

  switch (keysym.sym) {
  case SDLK_ESCAPE:
    printf("Escape key pressed, exiting\n");
    state->exit_trigger = 1;
    break;
  case SDLK_SPACE:
    state->pause = !state->pause;
    printf("Space key pressed, toggling pause to %d\n", state->pause);
    break;
  default:
    break;
  }
}

void handle_mouse_event(input_state_t *state, SDL_Event *event) {
  if (!state || !event)
    return;

  switch (event->type) {
  case SDL_MOUSEMOTION:
    state->mouse_x = event->motion.x;
    state->mouse_y = event->motion.y;
    break;
  case SDL_MOUSEBUTTONDOWN:
    if (event->button.button == SDL_BUTTON_LEFT) {
      state->mouse_clicked = 1;
      state->mouse_x = event->button.x;
      state->mouse_y = event->button.y;
    }
    break;
  case SDL_MOUSEBUTTONUP:
    if (event->button.button == SDL_BUTTON_LEFT) {
      state->mouse_clicked = 0;
    }
    break;
  }
}

void handle_events(input_state_t *state) {
  if (!state)
    return;

  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      handle_quit_event(state);
      break;
    case SDL_KEYDOWN:
      handle_keydown_event(state, e.key.keysym);
      break;
    case SDL_MOUSEMOTION:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      handle_mouse_event(state, &e);
      break;
    }
  }
}
