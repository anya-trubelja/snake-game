/*game.hpp -------
*
* Filename: game.hpp
* Author: Anya Trubelja
*
* Created: Sep 12 13:16:12 2022
* Last-Updated: June 01 01:35 2024
*
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
