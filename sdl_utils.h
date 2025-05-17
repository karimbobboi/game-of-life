#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include "/opt/homebrew/include/SDL2/SDL.h"
#include "/opt/homebrew/include/SDL2/SDL_ttf.h"

#define WINDOW_WIDTH 800 
#define WINDOW_HEIGHT 600

#define ACTUAL_WINDOW_WIDTH WINDOW_WIDTH
#define ACTUAL_WINDOW_HEIGHT WINDOW_HEIGHT + 80

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 40
#define BUTTON_X ((ACTUAL_WINDOW_WIDTH / 2) - 20)
#define BUTTON_Y (ACTUAL_WINDOW_HEIGHT - BUTTON_HEIGHT - 20)

// Initialize SDL window and renderer
int init_sdl(SDL_Window **window, SDL_Renderer **renderer);
void cleanup_sdl(SDL_Window *window);

// Initialize font
extern TTF_Font *font;
int init_ttf(void);
void cleanup_ttf(void);

void render_button(SDL_Renderer *renderer, int is_paused);
int is_button_clicked(int x, int y);

#endif