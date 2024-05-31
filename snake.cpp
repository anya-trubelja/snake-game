/* snake.cpp -------
 *
 * Filename: snake.cpp
 * Description:
 * Author: Anya Trubelja
*
* Created: Sep 12 13:16:12 2022
* Last-Updated: June 01 01:35 2024
*
*/

#include <cstring>
#include <cstdbool>
#include <cstdlib>
#include "snake.hpp"
#include "key.hpp"
#include <ncurses.h>

// Initialize snake
Snake *init_snake(int x, int y)
{
  Snake *head = create_tail(x, y);
  Snake *tail1 = create_tail(x - 1, y);
  Snake *tail2 = create_tail(x - 2, y);
  tail1->next = tail2;
  head->next = tail1;
  return head;
}

// Creates one tail
Snake *create_tail(int x, int y)
{
  Snake *snake = new Snake;
  snake->color[0] = 0;
  snake->color[1] = 0;
  snake->color[2] = 127;
  /* snake->color = {0, 0, 255}; */
  snake->symbol = '#';
  snake->next = NULL;
  snake->x = x;
  snake->y = y;
  return snake;
}

// Moves the snake in the input direction
Snake *move_snake(Snake* snake, int direction, int sizeChange)
{
    // Create a new head based on the current head's position
    Snake *new_head = create_tail(snake->x, snake->y);

    // Update the new head's position based on the direction
    switch (direction)
    {
    case UP:
        new_head->y -= 1;
        break;
    case DOWN:
        new_head->y += 1;
        break;
    case RIGHT:
        new_head->x += 1;
        break;
    case LEFT:
        new_head->x -= 1;
        break;
    }

    new_head->next = snake;

    // snake ate
    if (sizeChange > 0) {
       for (int i = 1; i < sizeChange; ++i) {
            Snake *extra_segment = create_tail(new_head->x, new_head->y);
            switch (direction)
            {
            case UP:
                extra_segment->y = new_head->y + i;
                break;
            case DOWN:
                extra_segment->y = new_head->y - i;
                break;
            case RIGHT:
                extra_segment->x = new_head->x - i;
                break;
            case LEFT:
                extra_segment->x = new_head->x + i;
                break;
            }
            // link extra segment with snake
            extra_segment->next = new_head->next;
            new_head->next = extra_segment;
        }
    } else if (sizeChange < 0) {
        // snake is shrinking
        for (int i = sizeChange; i <= 0; ++i) {
            new_head = remove_tail(new_head); 
        }
    }
    // snake didnt eat
    if (sizeChange == 0) {
        new_head = remove_tail(new_head);
    }

    return new_head;
}

Snake *remove_tail(Snake *snake)
{
  Snake *end = snake;
  while (end->next->next)
    end = end->next;
  free(end->next);
  end->next = NULL;
  return snake;
}

// draws the snake on the board
void draw_snake(Snake *snake)
{
  while (snake)
  {
    mvprintw(snake->y, snake->x, "%c", snake->symbol);
    snake = snake->next;
  }
}

// checks if it eats itself, if it does, then return true
bool eat_itself(Snake *snake)
{
  int size = 0;
  Snake *temp = snake;
  // Count the size of the snake
  while (temp != NULL) {
    size++;
    temp = temp->next;
  }

  // Only check for self-collision if the size is at least 3
  if (size >= 4) {
    Snake *head = snake;
    Snake *current = snake->next;
    while (current != NULL) {
      if (head->x == current->x && head->y == current->y) {
        return true; // has eaten itself
      }
      current = current->next;
    }
  }
  return false; 
}

int len(Snake* snake) {
    int length = 0;
    while (snake != NULL) {
        length++;
        snake = snake->next;
    }
    return length;
}