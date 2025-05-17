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

// Slider
#define SLIDER_WIDTH 200
#define SLIDER_HEIGHT 20
#define SLIDER_X ACTUAL_WINDOW_WIDTH - SLIDER_WIDTH - 10
#define SLIDER_Y (ACTUAL_WINDOW_HEIGHT - SLIDER_HEIGHT - 30)
#define SLIDER_HANDLE_WIDTH 20
#define MIN_INTERVAL 100    // Fastest
#define MAX_INTERVAL 2000   // Slowest
void render_slider(SDL_Renderer *renderer, int current_interval);
int is_slider_clicked(int x, int y);
int update_slider_value(int mouse_x);

// Start/Stop button
void render_button(SDL_Renderer *renderer, int is_paused);
int is_button_clicked(int x, int y);

#endif