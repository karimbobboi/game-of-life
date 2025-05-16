#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "logic.h"
#include "sdl_utils.h"

char **grid = NULL;
struct Block *block = NULL;
int *xlist = NULL;
int *ylist = NULL;

int grid_rows = 30;  // default value
int grid_cols = 40;  // default value
int live_cells = 0;
int steps;

// Initialise dynamic arrays
int init_grid(int rows, int cols){
    // Allocate grid
    grid = malloc(rows * sizeof(char*));
    if(!grid) return 0;
    
    for(int i = 0; i < rows; i++){
        grid[i] = malloc(cols * sizeof(char));
        if(!grid[i]){
            // Cleanup if allocation fails
            for(int j = 0; j < i; j++){
                free(grid[j]);
            }
            free(grid);
            return 0;
        }
    }
    
    // Allocate other arrays
    block = malloc(rows * cols * sizeof(struct Block));
    xlist = malloc(rows * cols * sizeof(int));
    ylist = malloc(rows * cols * sizeof(int));
    
    if (!block || !xlist || !ylist) {
        free_grid();
        return 0;
    }
    
    return 1;
}

// Free all dynamic memory
void free_grid(void){
    if (grid) {
        for(int i = 0; i < grid_rows; i++){
            free(grid[i]);
        }
        free(grid);
        grid = NULL;
    }
    
    free(block);
    free(xlist);
    free(ylist);
    block = NULL;
    xlist = NULL;
    ylist = NULL;
}

void fill(){
    for(int i = 0; i < grid_rows; i++){
        for(int j = 0; j < grid_cols; j++){
            grid[i][j] = ' ';
        }
    }
}

// Parses a space-separated string of numbers into an integer array
void separate(char s[30], int lis[100]) {
    int n = 0, numOflives = 0;
    char numSub[3];
    
    for(int i=0; i <= strlen(s); i++) {
        if(s[i] == ' ' || i == strlen(s)){
            lis[numOflives] = atoi(numSub);
            numOflives++;
            n = 0;
            continue;
        }
    
        numSub[n] = s[i];
        n++;
    }
    live_cells = numOflives;

    return;
}

// Displays the current state of the grid to console
void showGrid(){
    for(int i=0; i < grid_rows; i++){
        for(int j=0; j <= grid_cols; j++){
            if(j == grid_cols){
                printf("\n");
                continue;
            }
            printf("%c", grid[i][j]);
        }
    }
}

// Loads the initial game state from a file
int loadState(FILE *fp) {
    char str[30];

    if(!init_grid(grid_rows, grid_cols)){
        printf("Failed to allocate memory\n");
        return 0;
    }

    fp = fopen("specs.txt", "r");
    if (!fp) {
        printf("Error: Could not open specs.txt\n");
        return 0;
    }
    
    // Try to read from file
    if (fgets(str, 30, fp) != NULL) {
        str[strcspn(str, "\n")] = 0;
        separate(str, xlist);
        
        if (fgets(str, 30, fp) != NULL) {
            str[strcspn(str, "\n")] = 0;
            separate(str, ylist);
            
            fscanf(fp, "%d", &steps);
        }
    }
    
    fclose(fp);

    // Initialize grid
    fill();
    
    // Set up initial state
    for (int q=0; q < live_cells; q++){
        if (xlist[q] < grid_cols && ylist[q] < grid_rows){
            block[q].col = xlist[q];    
            block[q].row = ylist[q];   
            block[q].body = 'O';
            grid[block[q].row][block[q].col] = block[q].body;
        }
    }
    
    return 1;
}

// Saves the current game state to final_state.txt
int saveState(FILE *fp){
    int n = 0;

    fp = fopen("final_state.txt", "w");
    if(fp){
        while (n < live_cells){
            fprintf(fp, "position %d, x: %d y: %d\n", n, block[n].col, block[n].row);
            n++;
        }
        fprintf(fp, "Total Live Cells: %d", live_cells);
        fclose(fp);

        return 1;
    }

    return 0;
}

// Determines if a cell should live or die based on Conway's rules

int judge(int row, int col){
    if(row < 0 || col < 0 || row >= grid_rows || col >= grid_cols){
        return 0;
    }

    int neighbour = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            
            int newRow = row + i;
            int newCol = col + j;
            
            // Check bounds
            if (newRow >= 0 && newRow < grid_rows && 
                newCol >= 0 && newCol < grid_cols) {
                if (grid[newRow][newCol] == 'O') {
                    neighbour++;
                }
            }
        }
    }

    if (grid[row][col] == ' ') {
        return (neighbour == 3) ? 3 : 0;
    } else {
        return (neighbour == 2 || neighbour == 3) ? neighbour : 0;
    }
}

// Removes a cell from the list of live cells
int kill(int n){
    int a = 0, result = 0;
    int sub = n;

    if(sub >= live_cells){
        printf("Invalid number\n");
        return result;
    }
    if(sub == live_cells){
        block[sub].col = -100;
        block[sub].row = -100;
        block[sub].body = ' ';
    }

    while(a < live_cells){
        block[sub+a].col = block[(sub + a) + 1].col;
        block[sub+a].row = block[(sub + a) + 1].row;
        block[sub+a].body = block[(sub + a) + 1].body;
        a++;
    }
    result = 1;
    return result;
}

/**
 * Processes all cells and updates their states based on neighbor counts
 * Returns 1 on success, 0 if no live cells remain
 */
int lifeOrDeath(){
    int no = 0, result = 0;

    if(live_cells <= 0){
        printf("Lives less than 0\n");
        return -1;
    }
    
    for(int row=0; row < grid_rows; row++){
        for(int col=0; col < grid_cols; col++){
            if(grid[row][col] == ' ' && judge(row, col) == 3){
                block[live_cells].row = row;
                block[live_cells].col = col;
                block[live_cells].body = ' ';
                live_cells++;
            }
        }
    }

    while(no < live_cells){
        if(judge(block[no].row, block[no].col) == 0){
            kill(no);
            no--;
            live_cells--;
        }
        no++;
    }
    no = 0;
    fill();
    while(no < live_cells){
        block[no].body = 'O';
        grid[block[no].row][block[no].col] = block[no].body;
        no++;
    }
    
    result = 1;
    return result;
}

// Sets up a default initial pattern for testing
void test_state() {    
    int glider[5][2] = {
        {0, 1},
        {0, 2},
        {0, 3},
        {1, 2},
        {2, 2}
    };

    live_cells = 5;
    
    for(int i = 0; i < live_cells; i++){
        block[i].row = glider[i][0];
        block[i].col = glider[i][1];
        block[i].body = 'O';
        grid[block[i].row][block[i].col] = block[i].body;
    }
}
