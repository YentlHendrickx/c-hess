#ifndef CONFIG_H
#define CONFIG_H

// Screen dimensions
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 1280

// Board configuration
#define BOARD_SIZE 8
#define BOARD_OFFSET 20
#define BOARD_BORDER_WIDTH 2

// Piece configuration
#define PIECE_WIDTH 16
#define PIECE_HEIGHT 32
#define PIECE_SCALING 4.5

// File paths
#define ASSETS_PATH "assets/pieces/"
#define SPRITE_SHEET_SUFFIX "-sheet.png"

// Game timing
#define TARGET_FPS 60
#define PAUSED_FPS 30
#define FPS_UPDATE_INTERVAL 1000

// Colors (RGB values)
#define COLOR_BLACK 0, 0, 0
#define COLOR_WHITE 255, 255, 255
#define COLOR_LIGHT_GRAY 240, 240, 240
#define COLOR_DARK_GRAY 180, 180, 180

// Error codes
typedef enum {
    ERROR_NONE = 0,
    ERROR_SDL_INIT,
    ERROR_WINDOW_CREATE,
    ERROR_SURFACE_CREATE,
    ERROR_MEMORY_ALLOC,
    ERROR_FILE_LOAD,
    ERROR_INVALID_INPUT
} ErrorCode;

#endif // CONFIG_H
