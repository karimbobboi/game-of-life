#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "logic.h"

int xlist[100];
int ylist[100];

int sizeX;
int sizeY;
char grid[100][100];
struct Block block[1000];

int Live_Cells;
int steps;

// Parses a space-separated string of numbers into an integer array
void separate(char s[30],int lis[100]){
    int n = 0, numOflives = 0;
    char numSub[3];
    
    for(int i=0; i<=strlen(s); i++){
        if(s[i] == ' ' || i == strlen(s)){
            lis[numOflives] = atoi(numSub);
            numOflives++;
            n = 0;
            continue;
        }
    
        numSub[n] = s[i];
        n++;
    
    }
    Live_Cells = numOflives;

    return;
}

// Initializes the grid with empty spaces
void fill(){
    for(int i=0; i<sizeX; i++){
        for(int j=0; j<sizeY; j++){
            grid[i][j] = ' ';
        }
    } 
}

// Displays the current state of the grid to console
void showGrid(){
    for(int i=0; i<sizeX; i++){
        for(int j=0; j<=sizeY; j++){
            if(j==sizeY){
                printf("\n");
                continue;
            }
            printf("%c",grid[i][j]);
        }
    }
}

// Loads the initial game state from a file
int loadState(FILE *fp) {
    char str[30];

    // Retrieve initial cell setup from specs.txt
    fp = fopen("specs.txt", "r");
    if (!fp) {
        printf("Error: Could not open specs.txt\n");
        return 0;
    }

    // Initialize default values
    sizeX = 20;  // Default size
    sizeY = 40;  // Default size
    
    // Try to read from file
    if (fgets(str, 30, fp) != NULL) {
        str[strcspn(str, "\n")] = 0;
        separate(str, xlist);
        
        if (fgets(str, 30, fp) != NULL) {
            str[strcspn(str, "\n")] = 0;
            separate(str, ylist);
            
            fscanf(fp, "%d %d\n%d", &sizeX, &sizeY, &steps);
        }
    }
    
    fclose(fp);

    // Initialize grid
    fill();
    
    // Set up initial state
    for(int q = 0; q < Live_Cells; q++) {
        if (xlist[q] < sizeX && ylist[q] < sizeY) {  // Bounds checking
            block[q].x = xlist[q];
            block[q].y = ylist[q];
            block[q].body = 'O';
            grid[block[q].x][block[q].y] = block[q].body;
        }
    }
    
    return 1;
}

// Saves the current game state to final_state.txt
int saveState(FILE *fp){
    int n = 0;

    fp = fopen("final_state.txt", "w");
    if(fp){
        while(n < Live_Cells){
            fprintf(fp, "position %d, x: %d y: %d\n", n, block[n].x, block[n].y);
            n++;
        }
        fprintf(fp, "Total Live Cells: %d", Live_Cells);
        fclose(fp);

        return 1;
    }

    return 0;
}

// Determines if a cell should live or die based on Conway's rules
int judge(int row, int col){
    int neighbour = 0;
    int nr = 2, nc = 2;
    int sub = 0;

    if(row >= sizeX || col >= sizeY){
        printf("Invalid row or column\n");
        return -1;
    }


    while(sub < Live_Cells){
        if(block[sub].x == row && block[sub].y == col && block[sub].body == ' '){
            return -1;
        }
        sub++;
    }

    if(row == 0 || row == sizeX-1){
        row = row + 1;
        nr = 1;
    }
    if(col == 0 || col == sizeY-1){
        col = col + 1;
        nc = 1;
    }

    for (int i = row - 1; i < row + nr; i++){
        for (int j = col - 1; j < col + nc; j++){
            if (j == col && i == row){
                continue;
            }
            if (grid[i][j] == 'O'){
                neighbour++;
            }
        }
    }

    if(grid[row][col] == ' ' ){
        if(neighbour == 3){
            return neighbour;}
        else{
            return 0;
        }
    }

    if(neighbour < 2){
        neighbour = 0;
    }

    else if(neighbour > 3){
        neighbour = 0;
    }

    else if(neighbour == 2 || neighbour == 3){
        return neighbour;
    }

    return neighbour;
}

// Removes a cell from the list of live cells
int kill(int n){
    int a = 0, result = 0;
    int sub = n;

    if(sub >= Live_Cells){
        printf("Invalid number\n");
        return result;
    }
    if(sub == Live_Cells){
        block[sub].x = -100;
        block[sub].y = -100;
        block[sub].body = ' ';
    }


    while(a < Live_Cells){
        block[sub+a].x = block[(sub+a)+1].x;
        block[sub+a].y = block[(sub+a)+1].y;
        block[sub+a].body = block[(sub+a)+1].body;
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
    int s1 = 20, s2 = 40;

    if(Live_Cells <= 0){
        printf("Lives less than 0\n");
        return 0;
    }
    
    for(int i=0; i<s1; i++){
        for(int j=0; j < s2; j++){
            if(grid[i][j] == ' ' && judge(i, j) == 3){
                block[Live_Cells].x = i;
                block[Live_Cells].y = j;
                block[Live_Cells].body = ' ';
                Live_Cells++;
            }
        }
    }

    while(no < Live_Cells){
        if(judge(block[no].x, block[no].y) == 0){
            kill(no);
            no--;
            Live_Cells--;
        }
        no++;
    }
    no = 0;
    fill();
    while(no < Live_Cells){
        block[no].body = 'O';
        grid[block[no].x][block[no].y] = block[no].body;
        no++;
    }
    
    result = 1;
    return result;
}

// Sets up a default initial pattern for testing
void test_state(){
    block[0].x = 0;
    block[0].y = 5;
    block[0].body = 'O';
    grid[block[0].x][block[0].y] = block[0].body;

    block[1].x = 0;
    block[1].y = 6;
    block[1].body = 'O';
    grid[block[1].x][block[1].y] = block[1].body;

    block[2].x = 0;
    block[2].y = 4;
    block[2].body = 'O';
    grid[block[2].x][block[2].y] = block[2].body;

    block[3].x = 1;
    block[3].y = 5;
    block[3].body = 'O';
    grid[block[3].x][block[3].y] = block[3].body;

    block[4].x = 2;
    block[4].y = 5;
    block[4].body = 'O';
    grid[block[4].x][block[4].y] = block[4].body;
    
    Live_Cells = 5;
    
}
