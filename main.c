#include <stdlib.h>
#include <stdio.h>
#include "logic.h"
#include "sdl_utils.h"

#define CELL_WIDTH  (WINDOW_WIDTH / grid_cols)
#define CELL_HEIGHT (WINDOW_HEIGHT / grid_rows)

int main(int argc, char const *argv[]) {   
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    
    // Initialise grid first!
    if (!init_grid(grid_rows, grid_cols)) {
        fprintf(stderr, "Grid initialisation failed!\n");
        return 1;
    }

    // Initialise SDL
    if (!init_sdl(&window, &renderer)) {
        fprintf(stderr, "SDL initialization failed!\n");
        free_grid();  // Clean up if SDL fails
        return 1;
    }

    // Initialise font
    if (!init_ttf()) {
        fprintf(stderr, "Font initialization failed!\n");
        // Clean up if font init fails
        free_grid(); 
        cleanup_sdl(window);  
        return 1;
    }

    // Initialise game state
    if (!loadState(NULL)) {
        printf("Using default pattern\n");
        fill();
        test_state();
    }

    // Main game loop
    SDL_Event event;
    int running = 1;
    Uint32 lastUpdate = SDL_GetTicks();
    int is_paused = 0;

    int UPDATE_INTERVAL = 1000;
    int dragging_slider = 0;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN){
                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                if(is_button_clicked(mouse_x, mouse_y)){
                    is_paused = !is_paused;
                }
                else if(is_slider_clicked(mouse_x, mouse_y)){
                    dragging_slider = 1;
                    UPDATE_INTERVAL = update_slider_value(mouse_x);
                }
            }

            if(event.type == SDL_MOUSEBUTTONDOWN){
                dragging_slider = 0;
            }

            if (event.type == SDL_MOUSEMOTION && dragging_slider) {
                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                UPDATE_INTERVAL = update_slider_value(mouse_x);
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        // Manual advance
                        lifeOrDeath();
                        break;
                    case SDLK_r:
                        // Reset to initial state
                        fill();
                        test_state();
                        break;
                }
            }
        }
        
        // Automatic update every UPDATE_INTERVAL milliseconds
        Uint32 currentTime = SDL_GetTicks();
        if (!is_paused && currentTime - lastUpdate >= UPDATE_INTERVAL) {
            // End simulation if no living cells remain
            if(lifeOrDeath() == -1){
                running = 0;
            }
            lastUpdate = currentTime;
        }
        
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 123, 179, 105, 255); // Background color
        SDL_RenderClear(renderer);
        
        // Draw grid lines
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        // Horizontal lines
        for (int row = 0; row <= grid_rows; row++) {
            SDL_RenderDrawLine(renderer, 0, row * CELL_HEIGHT, 
                             WINDOW_WIDTH, row * CELL_HEIGHT);
        }
        // Vertical lines
        for (int col = 0; col <= grid_cols; col++) {
            SDL_RenderDrawLine(renderer, col * CELL_WIDTH, 0, 
                             col * CELL_WIDTH, WINDOW_HEIGHT);
        }
        
        // Draw cells
        SDL_SetRenderDrawColor(renderer, 141, 85, 36, 255);
        for(int row = 0; row < grid_rows; row++) {
            for(int col = 0; col < grid_cols; col++) {
                if(grid[row][col] == 'O') {
                    SDL_Rect cell = {
                        col * CELL_WIDTH + 1,   // x position
                        row * CELL_HEIGHT + 1,  // y position
                        CELL_WIDTH - 2,         // width
                        CELL_HEIGHT - 2         // height
                    };
                    SDL_RenderFillRect(renderer, &cell);
                }
            }
        }
        
        render_button(renderer, is_paused); // Draw start/pause button
        render_slider(renderer, UPDATE_INTERVAL); // Draw slider
        SDL_RenderPresent(renderer);
        
        SDL_Delay(16);
    }
    
    // Cleanup
    cleanup_ttf();
    cleanup_sdl(window);
    free_grid();
    return 0;
}