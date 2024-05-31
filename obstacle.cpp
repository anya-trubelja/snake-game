#include "obstacle.hpp"
#include <cstdlib>
#include <ncurses.h>
#include <ctime>

Obstacle* create_obstacle(int x, int y, int width, int height, char symbol) {
    Obstacle* new_obstacle = (Obstacle*)malloc(sizeof(Obstacle));
    new_obstacle->x = x;
    new_obstacle->y = y;
    new_obstacle->width = width;
    new_obstacle->height = height;
    new_obstacle->symbol = symbol;
    new_obstacle->next = NULL;
    return new_obstacle;
}

void generate_obstacles(Obstacle*& obstacles, int count, int game_width, int game_height, int x_offset, int y_offset) {
    srand(time(0)); // Ensure random numbers are generated
    for(int i = 0; i < count; i++) {
        int width = rand() % 5 + 1; // Random width from 1 to 5
        int height = rand() % 3 + 1; // Random height from 1 to 3
        int x = rand() % (game_width - width - 2) + x_offset + 1;
        int y = rand() % (game_height - height - 2) + y_offset + 1;
        char symbol = '$'; // Example symbol for the obstacle

        Obstacle* new_obstacle = (Obstacle*)malloc(sizeof(Obstacle));
        new_obstacle->x = x;
        new_obstacle->y = y;
        new_obstacle->width = width;
        new_obstacle->height = height;
        new_obstacle->symbol = symbol;
        new_obstacle->next = NULL;

        // Add the new obstacle to the list of obstacles
        if (obstacles == NULL) {
            obstacles = new_obstacle;
        } else {
            Obstacle* temp = obstacles;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = new_obstacle;
        }
    }
}


void draw_obstacles(Obstacle* obstacles) {
    while(obstacles) {
        for(int i = 0; i < obstacles->width; i++) {
            for(int j = 0; j < obstacles->height; j++) {
                attron(COLOR_PAIR(5)); // yellow
                mvprintw(obstacles->y + j, obstacles->x + i, "%c", obstacles->symbol);
                attroff(COLOR_PAIR(5)); // back to black
            }
        }
        obstacles = obstacles->next;
    }
}

bool obstacle_exists(Obstacle* obstacles, int x, int y) {
    while(obstacles) {
        for(int i = 0; i < obstacles->width; i++) {
            for(int j = 0; j < obstacles->height; j++) {
                if (obstacles->x + i == x && obstacles->y + j == y) {
                    return true;
                }
            }
        }
        obstacles = obstacles->next;
    }
    return false;
}