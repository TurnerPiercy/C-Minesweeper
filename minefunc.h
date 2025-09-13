/*
NAME: Turner Piercy
NetID: tpiercy
DATE: 5/2/2025
ASSIGNMENT: Lab 11 - Final Project: Minesweeper
PURPOSE: Header
*/

#include "gfx2.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
                     // ~~ DEFAULT VALUES ~~~
#define TILE_SIZE 18 // 18 pixels
#define GRID_ROWS 16 // 16
#define GRID_COLS 30 // 30
#define HEADER_HEIGHT 60
#define WINDOW_WIDTH (GRID_COLS * TILE_SIZE)
#define WINDOW_HEIGHT (GRID_ROWS * TILE_SIZE + HEADER_HEIGHT)
#define NUM_MINES 50 // 50

typedef struct {
    int is_mine;
    int is_revealed;
    int is_flagged;
    int neighbor_mines;
} Tile;
typedef struct {
  int total_flags;
  int game_over;
} GameState;

void drawGrid(Tile[GRID_ROWS][GRID_COLS]);
void drawHeader(GameState *);
void handleClick(int, int, Tile[GRID_ROWS][GRID_COLS], int *, char, GameState *);
void resetGame(Tile[GRID_ROWS][GRID_COLS], int *, GameState *);
void placeMines(int, int, Tile[GRID_ROWS][GRID_COLS]);
void calcNeighborMines(Tile[GRID_ROWS][GRID_COLS]);
void revealTile(int, int, Tile[GRID_ROWS][GRID_COLS]);
void revealAllMines(Tile board[GRID_ROWS][GRID_COLS]);
int checkWin(Tile board[GRID_ROWS][GRID_COLS]);
