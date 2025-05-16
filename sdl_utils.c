#include "sdl_utils.h"

int init_sdl(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 0;
    }

    *window = SDL_CreateWindow(
        "Conway's Game of Life",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        ACTUAL_WINDOW_WIDTH,
        ACTUAL_WINDOW_HEIGHT,
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

void render_button(SDL_Renderer *renderer, int is_paused){
    SDL_Rect button = {BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &button);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &button);
}

int is_button_clicked(int x, int y){
    return (x >= BUTTON_X && x <= BUTTON_X + BUTTON_WIDTH &&
            y >= BUTTON_Y && y <= BUTTON_Y + BUTTON_HEIGHT);
}