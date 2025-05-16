#ifndef LOGIC_GUARD__H 
#define LOGIC_GUARD__H
#include <stdio.h>

struct Block {
    int row;    
    int col;   
    char body;
};

extern char **grid;
extern struct Block *block;
extern int *xlist;
extern int *ylist;

extern int grid_rows;    // height of grid
extern int grid_cols;    // width of grid
extern int live_cells;   // number of live cells

// Function declarations
int init_grid(int rows, int cols);
void free_grid(void);
void fill();
void showGrid();
void test_state();
int judge(int row, int col);
int kill(int n);
int lifeOrDeath();
int loadState(FILE *fp);
int saveState(FILE *fp);
void separate(char s[30],int lis[100]);

extern int steps;

#endif