/*food.hpp -------
*
* Filename: food.hpp
* Author: Anya Trubelja
*
* Created: Sep 12 13:16:12 2022
* Last-Updated: June 01 01:35 2024
*
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
