#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include "/opt/homebrew/include/SDL2/SDL.h"

#define WINDOW_WIDTH 800 
#define WINDOW_HEIGHT 600

// Initialize SDL window and renderer
int init_sdl(SDL_Window **window, SDL_Renderer **renderer);
void cleanup_sdl(SDL_Window *window);

#endif