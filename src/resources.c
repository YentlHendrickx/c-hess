#include "resources.h"
#include <stdio.h>
#include <string.h>

ResourceManager g_resources = {0};

ErrorCode init_resources(void) {
  // Initialize all sprite sheets
  g_resources.white_default_sheet = IMG_Load("assets/pieces/white-sheet.png");
  if (!g_resources.white_default_sheet) {
    fprintf(stderr, "Failed to load white default sprite sheet: %s\n",
            SDL_GetError());
    return ERROR_FILE_LOAD;
  }

  g_resources.black_default_sheet = IMG_Load("assets/pieces/black-sheet.png");
  if (!g_resources.black_default_sheet) {
    fprintf(stderr, "Failed to load black default sprite sheet: %s\n",
            SDL_GetError());
    SDL_FreeSurface(g_resources.white_default_sheet);
    return ERROR_FILE_LOAD;
  }

  g_resources.white_wood_sheet = IMG_Load("assets/pieces/white-wood-sheet.png");
  if (!g_resources.white_wood_sheet) {
    fprintf(stderr, "Failed to load white wood sprite sheet: %s\n",
            SDL_GetError());
    cleanup_resources();
    return ERROR_FILE_LOAD;
  }

  g_resources.black_wood_sheet = IMG_Load("assets/pieces/black-wood-sheet.png");
  if (!g_resources.black_wood_sheet) {
    fprintf(stderr, "Failed to load black wood sprite sheet: %s\n",
            SDL_GetError());
    cleanup_resources();
    return ERROR_FILE_LOAD;
  }

  g_resources.initialized = 1;
  return ERROR_NONE;
}

void cleanup_resources(void) {
  if (g_resources.white_default_sheet) {
    SDL_FreeSurface(g_resources.white_default_sheet);
    g_resources.white_default_sheet = NULL;
  }
  if (g_resources.black_default_sheet) {
    SDL_FreeSurface(g_resources.black_default_sheet);
    g_resources.black_default_sheet = NULL;
  }
  if (g_resources.white_wood_sheet) {
    SDL_FreeSurface(g_resources.white_wood_sheet);
    g_resources.white_wood_sheet = NULL;
  }
  if (g_resources.black_wood_sheet) {
    SDL_FreeSurface(g_resources.black_wood_sheet);
    g_resources.black_wood_sheet = NULL;
  }
  g_resources.initialized = 0;
}

SDL_Surface *get_piece_sprite(int piece_type, int color, int theme) {
  if (!g_resources.initialized) {
    return NULL;
  }

  // Validate inputs
  if (piece_type < PAWN || piece_type > KING) {
    return NULL;
  }
  if (color != WHITE && color != BLACK) {
    return NULL;
  }
  if (theme != THEME_DEFAULT && theme != THEME_WOOD) {
    return NULL;
  }

  // Select appropriate sprite sheet
  SDL_Surface *sheet = NULL;
  if (color == WHITE) {
    sheet = (theme == THEME_WOOD) ? g_resources.white_wood_sheet
                                  : g_resources.white_default_sheet;
  } else {
    sheet = (theme == THEME_WOOD) ? g_resources.black_wood_sheet
                                  : g_resources.black_default_sheet;
  }

  return sheet;
}

const char *get_sprite_filename(int color, int theme) {
  static char filename[256];
  strcpy(filename, ASSETS_PATH);

  if (color == WHITE) {
    strcat(filename, "white");
  } else if (color == BLACK) {
    strcat(filename, "black");
  } else {
    return NULL;
  }

  if (theme == THEME_WOOD) {
    strcat(filename, "-wood");
  }

  strcat(filename, SPRITE_SHEET_SUFFIX);
  return filename;
}
