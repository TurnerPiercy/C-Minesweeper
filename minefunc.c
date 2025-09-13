/*
NAME: Turner Piercy
NetID: tpiercy
DATE: 5/2/2025
ASSIGNMENT: Lab 11 - Final Project: Minesweeper
PURPOSE: Non-main Functions
*/

#include "minefunc.h"

// Draw the Minesweeper game board with gfx
void drawGrid(Tile board[GRID_ROWS][GRID_COLS]){
  for (int row = 0; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLS; col++) {
      Tile tile = board[row][col];

      int x = col * TILE_SIZE;
      int y = row * TILE_SIZE + HEADER_HEIGHT;

      // Draw revealed tiles
      if (tile.is_revealed) {
        // Light gray background
        gfx_color(200, 200, 200);
        gfx_fill_rectangle(x, y, TILE_SIZE, TILE_SIZE);

        // Border
        gfx_color(100, 100, 100);
        gfx_rectangle(x, y, TILE_SIZE, TILE_SIZE);

        if (tile.is_mine) {
          // Draw a black mine (circle)
          gfx_color(0, 0, 0);
          gfx_fill_circle(x + TILE_SIZE/2, y + TILE_SIZE/2, TILE_SIZE/4);
        }
        else if (tile.neighbor_mines > 0) {
          // Draw number
          int num = tile.neighbor_mines;

          // Color based on number (classic colors)
          switch (num) {
            case 1: gfx_color(0, 0, 255); break;         // Blue
            case 2: gfx_color(0, 128, 0); break;         // Green
            case 3: gfx_color(255, 0, 0); break;         // Red
            case 4: gfx_color(0, 0, 128); break;         // Dark Blue
            case 5: gfx_color(128, 0, 0); break;         // Maroon
            case 6: gfx_color(0, 128, 128); break;       // Teal
            case 7: gfx_color(0, 0, 0); break;           // Black
            case 8: gfx_color(128, 128, 128); break;     // Gray
          }

          char num_text[2];
          sprintf(num_text, "%d", num);
          gfx_text(x + TILE_SIZE / 3, y + TILE_SIZE * 2 / 3, num_text);
        }
      }
      else {
        // Draw unrevealed tile (dark gray)
        gfx_color(169, 169, 169); // Dark gray
        gfx_fill_rectangle(x, y, TILE_SIZE, TILE_SIZE);

        // Border
        gfx_color(100, 100, 100);
        gfx_rectangle(x, y, TILE_SIZE, TILE_SIZE);

        // Draw flag if flagged
        if (tile.is_flagged) {
          gfx_color(255, 0, 0);
          gfx_text(x + TILE_SIZE / 2 - 3, y + TILE_SIZE / 2 + 3, "F");
        }
      }
    }
  }
}



// Draw the UI header with gfx
void drawHeader(GameState *state) {
  // Background
  gfx_color(180, 180, 180);
  gfx_fill_rectangle(0, 0, WINDOW_WIDTH, HEADER_HEIGHT);
  
  // Draw flag counter in top left
  char counter[16];
  sprintf(counter, "Mines Remaining: %d", NUM_MINES - state->total_flags);
  gfx_color(0, 0, 0); // black text
  gfx_text(10, 20, counter);

  // Draw smiley face if the game is not over
  if (state->game_over == 0) {
    int face_x = WINDOW_WIDTH / 2 - 12;
    int face_y = HEADER_HEIGHT / 2 - 12;
    gfx_color(255, 255, 0);
    gfx_fill_circle(face_x + 12, face_y + 12, 12);
    gfx_color(0, 0, 0);
    gfx_circle(face_x + 12, face_y + 12, 12);
    gfx_fill_circle(face_x + 8, face_y + 10, 2);
    gfx_fill_circle(face_x + 16, face_y + 10, 2);
    gfx_arc(face_x + 6, face_y + 6, 12, 12, 225, 90);  // Smile arc
  }
  // Draw frowny face with X eyes if game over
  else if (state->game_over == 1) {
    int face_x = WINDOW_WIDTH / 2 - 12;
    int face_y = HEADER_HEIGHT / 2 - 12;
    // Face background (yellow circle)
    gfx_color(255, 255, 0);
    gfx_fill_circle(face_x + 12, face_y + 12, 12);
    // Face border (black outline)
    gfx_color(0, 0, 0);
    gfx_circle(face_x + 12, face_y + 12, 12);
    // Left X eye
    gfx_line(face_x + 6, face_y + 8, face_x + 8, face_y + 10);
    gfx_line(face_x + 8, face_y + 8, face_x + 6, face_y + 10);
    // Right X eye
    gfx_line(face_x + 16, face_y + 8, face_x + 18, face_y + 10);
    gfx_line(face_x + 18, face_y + 8, face_x + 16, face_y + 10);
    gfx_arc(face_x + 6, face_y + 14, 12, 12, 45, 90);  // Frown arc
  }
}


// Function to handle user mouse clicks on the game board
void handleClick(int x, int y, Tile board[GRID_ROWS][GRID_COLS], int *first_click, char mode, GameState *state) {
  if (y < HEADER_HEIGHT) { // If header is clicked
    int dx = x - WINDOW_WIDTH / 2;
    int dy = y - HEADER_HEIGHT / 2;
    if (dx * dx + dy * dy <= 12 * 12) { // Reset board if smiley face clicked
      resetGame(board, first_click, state);
    }
    return; // Exit function
  }

  if (state->game_over == 1) return; // Do not handle tile clicks if the game is over

  int row = (y - HEADER_HEIGHT) / TILE_SIZE;
  int col = x / TILE_SIZE;

  if (mode == 'f' && state->total_flags <= NUM_MINES) { // 'f' key input
    if (!board[row][col].is_revealed) { // Toggle flag
      board[row][col].is_flagged = !board[row][col].is_flagged;
      // Add or subtract total flag counter
      if (board[row][col].is_flagged == 1) state->total_flags++;
      else state->total_flags--;
      drawHeader(state); // Re-draw header with updated count
    }
  } else { // Left click
    // Check that the tile clicked is not already flagged or revealed
    if (!board[row][col].is_flagged && !board[row][col].is_revealed) {
      if (*first_click != 0) { // Special case for first click
        placeMines(row, col, board);
        calcNeighborMines(board);
        *first_click = 0;
      }
      if (board[row][col].is_mine) { // If a mine is clicked
        revealAllMines(board);
        state->game_over = 1; // Game over
        drawHeader(state); // Re-draw header with sad face
      } else {
        revealTile(row, col, board);
        if (checkWin(board) == 1) {
          state->game_over = 1;
          return;
        }
      }
    }
  }

  drawGrid(board); // Re-draw grid with new revealed tile(s)
}


void resetGame(Tile board[GRID_ROWS][GRID_COLS], int *first_click, GameState *state) {
  *first_click = 1;

  // Assign values to all tiles
  for (int row = 0; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLS; col++) {
      board[row][col].is_mine = 0;
      board[row][col].is_revealed = 0;
      board[row][col].is_flagged = 0;
      board[row][col].neighbor_mines = 0;
    }
  }

  // Assign values to game state struct
  state->total_flags = 0;
  state->game_over = 0;

  // Clear graphics display and draw new game
  gfx_clear();
  drawHeader(state);
  drawGrid(board);
  gfx_flush();
}


// Function to place mines on the board excluding a 3x3 area around the first click
void placeMines(int safe_row, int safe_col, Tile board[GRID_ROWS][GRID_COLS]) {
  int placed = 0; // Counter for mines placed

  while (placed < NUM_MINES) { // Place all mines
    int row = rand() % GRID_ROWS;
    int col = rand() % GRID_COLS;

    // Skip if mine already exists
    if (board[row][col].is_mine) continue;

    // Skip the 3x3 safe zone around the first click
    if (abs(row - safe_row) <= 1 && abs(col - safe_col) <= 1) continue;

    // Place the mine
    board[row][col].is_mine = 1;
    placed++;
  }
}


// Calculate number of neighboring mines for non-mine tiles
void calcNeighborMines(Tile board[GRID_ROWS][GRID_COLS]) {
  for (int row = 0; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLS; col++) {
      // Skip if tile is a mine
      if (board[row][col].is_mine) continue;

      int count = 0;
      // Loop through neigbor tiles
      for (int nr = -1; nr <= 1; nr++) {
        for (int nc = -1; nc <= 1; nc++) {
          int nb_row = row + nr;
          int nb_col = col + nc;
          if (nb_row < 0 || nb_row >= GRID_ROWS || nb_col < 0 || nb_col >= GRID_COLS)
            continue; // Skip if neighbor tile is outside of the board
          if (board[nb_row][nb_col].is_mine)
            count++; // Increment neighbor mine counter if neighbor tile is a mine
        }
      }
      board[row][col].neighbor_mines = count;
    }
  }
}


// Function to reveal current tile and recursively reveal all 8 neighbors
// if the tile has zero neighboring mines
void revealTile(int row, int col, Tile board[GRID_ROWS][GRID_COLS]) { 
  Tile *tile = &board[row][col];
  
  // Bounds check
  if (row < 0 || row >= GRID_ROWS || col < 0 || col >= GRID_COLS) return;

  // Base cases: already revealed or flagged
  if (tile->is_revealed || tile->is_flagged) return;

  tile->is_revealed = 1; // Reveal the tile

  // Another base case: if tile has any neighboring mines
  if (tile->neighbor_mines > 0) return;

  // Recurse on all 8 neighbors
  for (int nr = -1; nr <= 1; nr++) {
    for (int nc = -1; nc <= 1; nc++) {
      if (nr == 0 && nc == 0) continue; // Skip self
      revealTile(row + nr, col + nc, board);
    }
  }
}


// Reveal all mines (called when player loses)
void revealAllMines(Tile board[GRID_ROWS][GRID_COLS]) {
  for (int row = 0; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLS; col++) {
      if (board[row][col].is_mine) {
        board[row][col].is_revealed = 1;
      }
    }
  }
}


// Check for a win (no more unrevealed non-mine tiles) and display win screen if so
int checkWin(Tile board[GRID_ROWS][GRID_COLS]) {
  // Exit function if any unrevealed, non-mine tiles found
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      if (board[i][j].is_revealed == 0 && board[i][j].is_mine == 0) {
        return 0;
      }
    }
  }
  // Display win screen if there are no more unrevealed, non-mine tiles
  gfx_color(0, 0, 0);
  gfx_clear();
  gfx_color(0, 255, 0);  // Bright green
  gfx_text(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2, "Congratulations, you beat Minesweeper!");
  return 1;
}
