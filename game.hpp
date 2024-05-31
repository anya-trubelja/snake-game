/*game.hpp -------
*
* Filename: game.hpp
* Description:
* Author: Adeel Bhutta
* Maintainer:
* Created: Sat Sep 12 13:16:12 2022
* Last-Updated: September 12 16:51 2022
*
*/

/* Commentary:
*
*
*
*/

/* Change log:
*
*
*/

/*Copyright (c) 2022 Adeel Bhutta
*
* All rights reserved.
*
* Additional copyrights may follow
*/

#include <string>
#include "snake.hpp"
#include "food.hpp"
#include "obstacle.hpp"

enum State{INIT, ALIVE, DEAD, EXIT, PAUSED};

void game();
void reset_game(State& state, Snake*& snake, Food*& foods, Obstacle*& obstacles, int& obstacle_count, int difficultyLevel, int x_offset, int y_offset, int width, int height);
void display_best_scores(const std::string& filename);
void update_best_scores(const std::string& filename, int newScore);
