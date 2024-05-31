/*food.hpp -------
*
* Filename: food.hpp
* Description:
* Author: Adeel Bhutta
* Maintainer:
* Created: Sat Sep 12 13:21:55 2022
* Last-Updated: September 12 16:01 2022
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

#include <ncurses.h>

//Four types of food
//Either Increase, Decrease, IncreaseMore or DecreaseMore
enum Type {Increase, Decrease, IncreaseMore, DecreaseMore}; 

//Structure to hold properties of food
struct Food {
  int x;
  int y;
  char type;
  struct Food* next;
};

typedef struct Food Food; 

//Function prototypes 
void add_new_food(Food* foods, Food* new_food);
bool food_exists(Food* foods, int x, int y);
Food* create_food(int x, int y, enum Type type);
void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset);
void generate_initial_food(Food*& foods, int width, int height, int x_offset, int y_offset, int difficultyLevel);
enum Type food_type(Food* foods, int x, int y);
Food* remove_eaten_food(Food* foods, int x, int y);
void draw_food(Food *food);
