#ifndef LOGIC_GUARD__H 
#define LOGIC_GUARD__H
#include <stdio.h>

struct Block{
    int x;
    int y;
    char body;
};

void fill();
void showGrid();
void test_state();
int judge(int row, int col);
int kill(int n);
int lifeOrDeath();
int loadState(FILE *fp);
int saveState(FILE *fp);
void separate(char s[30],int lis[100]);

extern char grid[100][100];
extern struct Block block[1000];

int Live_Cells;
int steps;
int sizeX;
int sizeY;

int xlist[100];
int ylist[100];

#endif