#include <stdlib.h>
#include <stdio.h>
#include "logic.h"
#include "sdl_utils.h"

extern int sizeX, sizeY;
extern int Live_Cells;

int main(int argc, char const *argv[]) {   
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    
    // Initialize default values
    sizeX = 20;
    sizeY = 40;
    Live_Cells = 0;
    
    // Initialize SDL
    if (!init_sdl(&window, &renderer)) {
        fprintf(stderr, "SDL initialization failed!\n");
        return 1;
    }

    // Initialize game state
    if (!loadState(NULL)) {
        printf("Failed to load state, using default pattern\n");
        fill();  // Initialize empty grid
        test_state();   // Set up default pattern
    }

    // Main game loop
    SDL_Event event;
    int running = 1;
    Uint32 lastUpdate = SDL_GetTicks();
    const int UPDATE_INTERVAL = 500; // Update every 500ms
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
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
        if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
            lifeOrDeath();
            lastUpdate = currentTime;
        }
        
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 123, 179, 105, 255); // Background color
        SDL_RenderClear(renderer);
        
        // Draw grid lines
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int i = 0; i <= sizeX; i++) {
            SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, 
                             sizeY * CELL_SIZE, i * CELL_SIZE);
        }
        for (int j = 0; j <= sizeY; j++) {
            SDL_RenderDrawLine(renderer, j * CELL_SIZE, 0, 
                             j * CELL_SIZE, sizeX * CELL_SIZE);
        }
        
        // Draw cells
        SDL_SetRenderDrawColor(renderer, 141,85,36, 255);
        for(int i = 0; i < sizeX; i++) {
            for(int j = 0; j < sizeY; j++) {
                if(grid[i][j] == 'O') {
                    SDL_Rect cell = {
                        j * CELL_SIZE + 1,  // +1 for grid line spacing
                        i * CELL_SIZE + 1,  // +1 for grid line spacing
                        CELL_SIZE - 2,      // Adjust for grid line spacing
                        CELL_SIZE - 2       // Adjust for grid line spacing
                    };
                    SDL_RenderFillRect(renderer, &cell);
                }
            }
        }
        
        SDL_RenderPresent(renderer);
        
        SDL_Delay(16);
    }
    
    cleanup_sdl(window);
    return 0;
}