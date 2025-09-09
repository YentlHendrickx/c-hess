#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "config.h"

// Input state structure
typedef struct {
    int exit_trigger;
    int pause;
    int mouse_x;
    int mouse_y;
    int mouse_clicked;
    int key_pressed[256]; // Track key states
} InputState;

// Input handling functions
void init_input_state(InputState* state);
void handle_events(InputState* state);
void handle_quit_event(InputState* state);
void handle_keydown_event(InputState* state, SDL_Keysym keysym);
void handle_mouse_event(InputState* state, SDL_Event* event);

#endif // INPUT_H
