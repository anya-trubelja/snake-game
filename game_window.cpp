/* game_window.cpp -------
 *
 * Filename: game_window.cpp
 * Description:
 * Author: Adeel Bhutta
 * Maintainer:
 * Created: Tue Sep  6 14:10:06 2022
 * Last-Updated: June 4 11:00 2022
 * Keywords:
 * Compatibility:
 *
 */

/* Commentary:
 *
 *
 *
 */

/* Change log:
 * added void display_welcome_screen()
 *
 */

/* Copyright (c) 2022 Adeel Bhutta
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */

/* Code: */

#include <iostream>
#include <cstdlib>
#include <ncurses.h>
#include "game_window.hpp"

using namespace std; 

// Function to display the welcome screen
void display_welcome_screen(int difficultyLevel) {
    start_color(); // init color functionality
    init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);

    clear();
	attron(COLOR_PAIR(1)); 
    mvprintw(5, 10, "Welcome to the SNAKE GAME!");
	attroff(COLOR_PAIR(1));
    mvprintw(7, 10, "Scoring System:");
    mvprintw(8, 10, "  - Eat 'O' to increase score by 20 points.");
    mvprintw(9, 10, "  - Eat '+' to increase score by 40 points.");
	mvprintw(10, 10, "  - Eat 'X' to decrease score by 10 points.");
    mvprintw(11, 10, "  - Eat '-' to decrease score by 20 points.");
    mvprintw(12, 10, "Controls:");
    mvprintw(13, 10, "  - Use arrow keys to move the snake.");
    mvprintw(14, 10, "  - Press 'p' to pause.");
    mvprintw(15, 10, "  - Press 'q' to quit.");
    mvprintw(16, 10, "Press 's' to start the game.");
    mvprintw(17, 10, "Use LEFT and RIGHT arrow keys to change difficulty level.");
    const char* difficulty = (difficultyLevel == 0) ? "Easy" : (difficultyLevel == 1) ? "Hard" : "Advanced";
    

	mvprintw(19, 10, "Current Difficulty: ");    
    attron(COLOR_PAIR(2)); 
    mvprintw(19, 30, "%s", difficulty);
    attroff(COLOR_PAIR(2));

    refresh(); // Refresh the screen to display the text
}

gamewindow_t *init_GameWindow(int upper_left_x, int upper_left_y, int width, int height)
{
	gamewindow_t *g;
	g = (gamewindow_t *) malloc(sizeof(gamewindow_t));
	g->upper_left_x = upper_left_x;
	g->upper_left_y = upper_left_y;
	g->width = width;
	g->height = height;
	g->draw_char = '*';
	g->color[0] = 0;
	g->color[1] = 0;
	g->color[2] = 127;
	return (g);
}

void draw_Gamewindow(gamewindow_t *r)
{
	int row_counter, column_counter;

	// Draw Top of room
	for (row_counter = r->upper_left_x;
		 row_counter <= (r->upper_left_x + r->width);
		 row_counter++) {
		mvprintw(r->upper_left_y, row_counter, "%c", r->draw_char);
	}

	// Draw left side of room
	for (column_counter = r->upper_left_y;
		 column_counter <= (r->upper_left_y + r->height);
		 column_counter++) {
		mvprintw(column_counter, r->upper_left_x, "%c", r->draw_char);
	}

	// Draw right side of room
	for (column_counter = r->upper_left_y;
		 column_counter <= (r->upper_left_y + r->height);
		 column_counter++) {
		mvprintw(column_counter, (r->upper_left_x + r->width), "%c", r->draw_char);
	}

	// Draw Bottom of room
	for (row_counter = r->upper_left_x;
		 row_counter <= (r->upper_left_x + r->width);
		 row_counter++) {
		mvprintw(r->upper_left_y + r->height, row_counter, "%c", r->draw_char);
	}
}

gamewindow_t *changeGameWindow(int upper_left_x, int upper_left_y, int width, int height, gamewindow_t *r)
{
	r->upper_left_x = upper_left_x;
	r->upper_left_y = upper_left_y;

	if (width < 10)
		r->width = 10;
	else
		r->width = width;

	if (height < 10)
		r->height = 10;
	else
		r->height = height;

	return (r);
}

void undraw_Gamewindow(gamewindow_t *r)
{
	int row_counter, column_counter;

	// Undraw Top of room
	for (row_counter = r->upper_left_x;
		 row_counter <= (r->upper_left_x + r->width);
		 row_counter++) {
		mvprintw(r->upper_left_y, row_counter, " ");
	}

	// Undraw left side of room
	for (column_counter = r->upper_left_y;
		 column_counter<=(r->upper_left_y + r->height);
		 column_counter++) {
		mvprintw(column_counter, r->upper_left_x, " ");
	}

	// Undraw right side of room
	for (column_counter = r->upper_left_y;
		 column_counter <= (r->upper_left_y + r->height);
		 column_counter++) {
		mvprintw(column_counter, (r->upper_left_x + r->width), " ");
	}

	// Undraw Bottom of room
	for (row_counter = r->upper_left_x;
		 row_counter <= (r->upper_left_x + r->width);
		 row_counter++) {
		mvprintw(r->upper_left_y + r->height, row_counter, " ");
	}
}
/* room.c ends here */


