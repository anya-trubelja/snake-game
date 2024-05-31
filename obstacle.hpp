/* obstacle.hpp -------
 *
 * Filename: obstacle.hpp
 * Author: Anya Trubelja
*
* Created: Sep 12 13:16:12 2022
* Last-Updated: June 01 01:35 2024
*
*/

struct Obstacle {
  int x;
  int y;
  int width;
  int height;
  char symbol;
  struct Obstacle* next;
};

typedef struct Obstacle Obstacle;

Obstacle* create_obstacle(int x, int y, int width, int height, char symbol);
void generate_obstacles(Obstacle*& obstacles, int count, int game_width, int game_height, int x_offset, int y_offset);
void draw_obstacles(Obstacle* obstacles);
bool obstacle_exists(Obstacle* obstacles, int x, int y);