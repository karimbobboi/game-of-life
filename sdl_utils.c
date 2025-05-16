#include "sdl_utils.h"

int init_sdl(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 0;
    }

    *window = SDL_CreateWindow(
        "Conway's Game of Life",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    if (!*window) {
        return 0;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_SOFTWARE);
    if (!*renderer) {
        SDL_DestroyWindow(*window);
        return 0;
    }

    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(*renderer);
    SDL_RenderPresent(*renderer);

    return 1;
}

void cleanup_sdl(SDL_Window *window) {
    SDL_DestroyWindow(window);
    SDL_Quit();
}