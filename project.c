/*
NAME: Turner Piercy
NetID: tpiercy
DATE: 5/2/2025
ASSIGNMENT: Lab 11 - Final Project: Minesweeper
PURPOSE: Main Function
*/

#include "minefunc.h"

int main()
{
  int click_x, click_y;
  int first_click = 1;
  srand(time(NULL));

  gfx_open(WINDOW_WIDTH, WINDOW_HEIGHT, "Minesweeper");

  // Initialize a struct array for the tiles
  Tile board[GRID_ROWS][GRID_COLS]; 

  // Initialize game state struct
  GameState state;

  // Set up game (Draw board & assign vals to tiles)
  resetGame(board, &first_click, &state);

  // Game loop
  while (1) {
    if (gfx_event_waiting()) {
      char c = gfx_wait();
      if (c == 1 || c == 'f') {  // Left click or 'f' key
        click_x = gfx_xpos();
        click_y = gfx_ypos();
        handleClick(click_x, click_y, board, &first_click, c, &state);
      }
      else if (c == 'q') break;
    }
  }

  return 0;
}
