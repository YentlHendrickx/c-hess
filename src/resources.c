#include "resources.h"
#include <stdio.h>
#include <string.h>

resource_manager_t g_resources = {0};

ErrorCode init_resources(SDL_Renderer *renderer) {
  if (!renderer) {
    fprintf(stderr, "Renderer is NULL\n");
    return ERROR_INVALID_INPUT;
  }

  // Load surfaces first, then convert to textures
  SDL_Surface *temp_surface;

  // Load white default sprite sheet
  temp_surface = IMG_Load("assets/pieces/white-sheet.png");
  if (!temp_surface) {
    fprintf(stderr, "Failed to load white default sprite sheet: %s\n",
            SDL_GetError());
    return ERROR_FILE_LOAD;
  }
  g_resources.white_default_sheet =
      SDL_CreateTextureFromSurface(renderer, temp_surface);
  SDL_FreeSurface(temp_surface);
  if (!g_resources.white_default_sheet) {
    fprintf(stderr,
            "Failed to create texture from white default sprite sheet: %s\n",
            SDL_GetError());
    return ERROR_FILE_LOAD;
  }

  // Load black default sprite sheet
  temp_surface = IMG_Load("assets/pieces/black-sheet.png");
  if (!temp_surface) {
    fprintf(stderr, "Failed to load black default sprite sheet: %s\n",
            SDL_GetError());
    cleanup_resources();
    return ERROR_FILE_LOAD;
  }
  g_resources.black_default_sheet =
      SDL_CreateTextureFromSurface(renderer, temp_surface);
  SDL_FreeSurface(temp_surface);
  if (!g_resources.black_default_sheet) {
    fprintf(stderr,
            "Failed to create texture from black default sprite sheet: %s\n",
            SDL_GetError());
    cleanup_resources();
    return ERROR_FILE_LOAD;
  }

  // Load white wood sprite sheet
  temp_surface = IMG_Load("assets/pieces/white-wood-sheet.png");
  if (!temp_surface) {
    fprintf(stderr, "Failed to load white wood sprite sheet: %s\n",
            SDL_GetError());
    cleanup_resources();
    return ERROR_FILE_LOAD;
  }
  g_resources.white_wood_sheet =
      SDL_CreateTextureFromSurface(renderer, temp_surface);
  SDL_FreeSurface(temp_surface);
  if (!g_resources.white_wood_sheet) {
    fprintf(stderr,
            "Failed to create texture from white wood sprite sheet: %s\n",
            SDL_GetError());
    cleanup_resources();
    return ERROR_FILE_LOAD;
  }

  // Load black wood sprite sheet
  temp_surface = IMG_Load("assets/pieces/black-wood-sheet.png");
  if (!temp_surface) {
    fprintf(stderr, "Failed to load black wood sprite sheet: %s\n",
            SDL_GetError());
    cleanup_resources();
    return ERROR_FILE_LOAD;
  }
  g_resources.black_wood_sheet =
      SDL_CreateTextureFromSurface(renderer, temp_surface);
  SDL_FreeSurface(temp_surface);
  if (!g_resources.black_wood_sheet) {
    fprintf(stderr,
            "Failed to create texture from black wood sprite sheet: %s\n",
            SDL_GetError());
    cleanup_resources();
    return ERROR_FILE_LOAD;
  }

  g_resources.initialized = 1;
  return ERROR_NONE;
}

void cleanup_resources(void) {
  if (g_resources.white_default_sheet) {
    SDL_DestroyTexture(g_resources.white_default_sheet);
    g_resources.white_default_sheet = NULL;
  }
  if (g_resources.black_default_sheet) {
    SDL_DestroyTexture(g_resources.black_default_sheet);
    g_resources.black_default_sheet = NULL;
  }
  if (g_resources.white_wood_sheet) {
    SDL_DestroyTexture(g_resources.white_wood_sheet);
    g_resources.white_wood_sheet = NULL;
  }
  if (g_resources.black_wood_sheet) {
    SDL_DestroyTexture(g_resources.black_wood_sheet);
    g_resources.black_wood_sheet = NULL;
  }
  g_resources.initialized = 0;
}

SDL_Texture *get_piece_sprite(int piece_type, int color, int theme) {
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
  SDL_Texture *sheet = NULL;
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
