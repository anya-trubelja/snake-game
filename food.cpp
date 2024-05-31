/*food.cpp -------
*
* Filename: food.cpp
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

#include <cstdlib>
#include <ncurses.h>
#include <cstdbool>
#include "food.hpp"


//Create new food
Food* create_food(int x, int y, enum Type type){
    Food* new_food = (Food*)malloc(sizeof(Food));
     
    new_food->x = x;
    new_food->y = y;
    if (type == Increase){
        new_food->type = 'O';
    }
    else if(type == Decrease){
        new_food->type = 'X';
    }
    else if(type == IncreaseMore){
        new_food->type = '+';
    }
    else if(type == DecreaseMore){
        new_food->type = '-';
    }
    new_food->next = NULL;

    return new_food;
}

void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset){
    *food_x = (rand() % (width - 4)) + x_offset + 2;
    *food_y = (rand() % (height - 4)) + y_offset + 2;

    // dont spawn food on snake starting line so it doesnt instantly die
    if (*food_y == y_offset + (height / 2)) {
        (*food_y)++;
    }
}

void generate_initial_food(Food*& foods, int width, int height, int x_offset, int y_offset, int difficultyLevel) {
    int food_x, food_y;
    enum Type type;

    // Generate 10 foods
    generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
    type = static_cast<Type>(rand() % 4);
    foods = create_food(food_x, food_y, type);

    int multiplier = 1;

    if (difficultyLevel == 1) {
        multiplier = 2; // hard mode
    } else if (difficultyLevel == 2) {
        multiplier = 4; // advanced mode
    }

    for(int i = 1; i < 10 * multiplier; i++){
        generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
        while (food_exists(foods,food_x, food_y))
            generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
        type = static_cast<Type>(rand() % 4);
        Food* new_food = create_food(food_x, food_y, type);
        add_new_food(foods, new_food);
    }
}
//Check if food exists at coordinates
bool food_exists(Food* foods, int x, int y){
    Food* temp = foods;
    while(temp){
        if(temp->x == x && temp->y == y)
            return true;
        temp = temp->next;
    }
    return false;
}

//Add new food to end of food list
void add_new_food(Food* foods, Food* new_food){
    Food* temp = foods;
    while(temp->next) {
        temp = temp->next;
    }
    temp->next = new_food;
}


enum Type food_type(Food* foods, int x, int y){
    Food* temp = foods;
    while(temp != NULL) {
        if(temp->x == x && temp->y == y) {
            if(temp->type == 'O') return Increase;
            if(temp->type == 'X') return Decrease;
            if(temp->type == '+') return IncreaseMore;
            if(temp->type == '-') return DecreaseMore;
        }
        temp = temp->next;
    }
    return Increase;
}

Food* remove_eaten_food(Food* foods, int x, int y){
    Food* current = foods;
    Food* prev = NULL;
    while(current != NULL) {
        if(current->x == x && current->y == y) {
            if(prev == NULL) { // Food to remove is the first item
                foods = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return foods;
        }
        prev = current;
        current = current->next;
    }
    return foods; // Return the possibly modified list
}

// Display all the food
void draw_food(Food* foods) {
    Food* temp = foods;
    while(temp) {
        Type currentType = food_type(foods, temp->x, temp->y);

        if(currentType == Increase || currentType == IncreaseMore) {
            attron(COLOR_PAIR(3)); // green for good food
        } else {
            attron(COLOR_PAIR(4)); // red for bad food
        }

        mvprintw(temp->y, temp->x, "%c", temp->type); 

        if(currentType == Increase || currentType == IncreaseMore) {
            attroff(COLOR_PAIR(3));
        } else {
            attroff(COLOR_PAIR(4));
        }

        temp = temp->next;
    }
}