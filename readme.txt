Turner Piercy
CSE 20311
May 2, 2025

#############################
##                         ##
##   Lab 11: Minesweeper   ##
##                         ##
#############################

This project was intended to replicate the classic minesweeper game
available at freeminesweeper.org in C. Rules for the game can be
found at https://freeminesweeper.org/how-to-play-minesweeper.php

This C-based Minesweeper game implements a custom graphics layer built 
directly on top of the X11/Xlib libraries rather than relying on a h
igher-level graphics framework. All of the game’s visuals—grid, tiles,
mines, and UI elements—are drawn using low-level Xlib functions, which
makes the rendering lightweight and customizable. Since the project
depends on the X Window System, it requires an active X11 server to
display the graphics. On Windows, this can be done by running Xming
(or another X server) in the background before launching the game.

All general gameplay is the same, with only a few minor changes:

  1) To flag an unrevealed tile that you believe is a mine,
     hover your cursor over that tile and press the 'F' key
     (rather than right click)
  2) There is no question mark feature as found in the optional
     settings at freeminesweeper.org
