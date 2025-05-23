#include "sdl_utils.h"

TTF_Font* font = NULL;

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

int init_ttf(void){
    if(TTF_Init() < 0)
        return 0;
    font = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", 24);
    return 1;
}

void cleanup_ttf(void){
    if(font)
        TTF_CloseFont(font);
    TTF_Quit();
}

void render_button(SDL_Renderer *renderer, int is_paused){
    SDL_Rect button = {BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT};

    if(is_paused){
        SDL_SetRenderDrawColor(renderer, 0,255,0, 255);
        SDL_RenderFillRect(renderer, &button);

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawRect(renderer, &button);
    } else {
        SDL_SetRenderDrawColor(renderer, 220,20,60, 255);
        SDL_RenderFillRect(renderer, &button);

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawRect(renderer, &button);
    }

    if(font){
        const char* btn_text = is_paused ? "Start" : "Stop";
        SDL_Color text_color = {0, 0, 0, 255};

        SDL_Surface* text_surface = TTF_RenderText_Solid(font, btn_text, text_color);
        if(text_surface){
            SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
            if(text_texture){
                SDL_Rect text_rect = {
                    BUTTON_X + (BUTTON_WIDTH - text_surface->w) / 2,  // Center text
                    BUTTON_Y + (BUTTON_HEIGHT - text_surface->h) / 2,
                    text_surface->w,
                    text_surface->h
                };

                SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
                SDL_DestroyTexture(text_texture);
            }
            SDL_FreeSurface(text_surface);
        }
    }
}

int is_button_clicked(int x, int y){
    return (x >= BUTTON_X && x <= BUTTON_X + BUTTON_WIDTH &&
            y >= BUTTON_Y && y <= BUTTON_Y + BUTTON_HEIGHT);
}

void render_slider(SDL_Renderer *renderer, int current_interval){
    // Slider track
    SDL_Rect track = {SLIDER_X, SLIDER_Y, SLIDER_WIDTH, SLIDER_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &track);
    
    // Calculate handle position
    float position = 1.0f - (float)(current_interval - MIN_INTERVAL) / (MAX_INTERVAL - MIN_INTERVAL);
    int handle_x = SLIDER_X + (SLIDER_WIDTH - SLIDER_HANDLE_WIDTH) * position;
    
    // Draw handle
    SDL_Rect handle = {handle_x, SLIDER_Y - 5, SLIDER_HANDLE_WIDTH, SLIDER_HEIGHT + 10};
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &handle);
}

int is_slider_clicked(int x, int y){
    return (x >= SLIDER_X && x <= SLIDER_X + SLIDER_WIDTH &&
            y >= SLIDER_Y - 5 && y <= SLIDER_Y + SLIDER_HEIGHT + 5);
}

int update_slider_value(int mouse_x){
    float position = 1.0f - (float)((mouse_x + 420) - SLIDER_X) / SLIDER_WIDTH;
    // if (position < 0) position = 0;
    // if (position > 1) position = 1;
    
    return MIN_INTERVAL + (MAX_INTERVAL - MIN_INTERVAL) * position;
}