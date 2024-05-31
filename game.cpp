/*game.cpp -------
*
* Filename: game.cpp
* Author: Anya Trubelja
*
* Created: Sep 12 13:16:12 2022
* Last-Updated: June 01 01:35 2024
*
*/


#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ncurses.h>
#include "game_window.hpp"
#include "key.hpp"
#include "game.hpp"

using namespace std;

void game(){
    enum State state = INIT; // Set the initial state
    static int x_max, y_max; //Max screen size variables
    static int x_offset, y_offset; // distance between the top left corner of your screen and the start of the board
    gamewindow_t *window; // Name of the board
    Snake *snake; // The snake
    Food *foods,*new_food; // List of foods (Not an array)

    const int height = 30; 
    const int width = 70;
    char ch;

    struct timespec timeret;
    timeret.tv_sec = 0;
    timeret.tv_nsec = 999999999/4;


    int score = 0;
    bool startGame = false;

    int speedIncreaseThreshold = 100; // score threshold for speed increase

    srand(time(0)); // init random seed
    int last_direction; 

    int difficultyLevel = 0; // 0 for Easy, 1 for Hard and 2 for Advanced

    int lives = 3;

    double speedMultiplier = 1.0;

    int obstacle_count;
    Obstacle* obstacles = NULL;

    while(state != EXIT){
        int ch = get_char();

        if (ch == 'q' || ch == 'Q') { 
            state = EXIT; 
        }
        
        switch(state){
        case INIT:
            initscr();
            start_color();
            init_pair(3, COLOR_GREEN, COLOR_BLACK);
            init_pair(4, COLOR_RED, COLOR_BLACK);
            init_pair(5, COLOR_YELLOW, COLOR_BLACK);

            nodelay(stdscr, TRUE); //Dont wait for char
            noecho(); // Don't echo input chars
            getmaxyx(stdscr, y_max, x_max);
            keypad(stdscr, TRUE); // making keys work
            curs_set(0); // hide cursor
            timeout(100);

            display_welcome_screen(difficultyLevel);

            while (!startGame) {
                ch = get_char();
                if (ch == 's' || ch == 'S') {
                    startGame = true;
                    clear();
                } else if (ch == 'q' || ch == 'Q') {
                    state = EXIT;
                    break;
                } else if (ch == LEFT) {
                    difficultyLevel = max(0, difficultyLevel - 1);
                    display_welcome_screen(difficultyLevel);
                } else if (ch == RIGHT) {
                    difficultyLevel = min(2, difficultyLevel + 1);
                    display_welcome_screen(difficultyLevel);
                }
            }
	
            if (state == EXIT) {
                endwin(); 
                return;
            }

            obstacle_count = rand() % 8 + 3; // random num between 3 and 10

            switch(difficultyLevel) {
                case 0: // Easy
                    speedMultiplier = 1.0;
                    break;
                case 1: // Hard
                    speedMultiplier = 2.0; // 2x speed for Hard
                    obstacle_count *= 2;
                    break;
                case 2: // Advanced
                    speedMultiplier = 4.0; // 4x speed for Advanced
                    obstacle_count *= 4;
                    break;
            }

            timeret.tv_nsec = static_cast<long>(timeret.tv_nsec / speedMultiplier);

            
            // Setting height and width of the board
            x_offset = (x_max / 2) - (width / 2);
            y_offset = (y_max / 2) - (height / 2);
            
            //Init board
            window = init_GameWindow(x_offset, y_offset, width, height);
            draw_Gamewindow(window);

            // Init snake
            snake = init_snake(x_offset + (width / 2), y_offset + (height / 2));
            
            // Init foods
            generate_initial_food(foods, width - 2, height - 2, x_offset + 1, y_offset + 1, difficultyLevel); // avoid spawning food on the border

            // Init obstacles
            generate_obstacles(obstacles, obstacle_count, width, height, x_offset, y_offset);

            state = ALIVE;
            last_direction =  (rand() % 2) + 1; // either start moving left or right
            break;

        case ALIVE: {

            // pause game check
            if (ch == 'p' || ch == 'P') {
                state = PAUSED;
                break;
            }

            if (score / speedIncreaseThreshold > (speedMultiplier - 1.0)) {
                speedMultiplier *= 1.5; // increase speed by 1.5x
                timeret.tv_nsec = static_cast<long>(timeret.tv_nsec / speedMultiplier);
                speedIncreaseThreshold *= speedMultiplier;
            }

            int prevDirection = last_direction;
            
            // map boundary check
            if (snake->x < x_offset + 1 || snake->x >= x_offset + width || snake->y < y_offset + 1 || snake->y >= y_offset + height || obstacle_exists(obstacles, snake->x, snake->y)) {
                lives--;
                if (lives <= 0) {
                    state = DEAD;
                } else {
                    reset_game(state, snake, foods, obstacles, obstacle_count, difficultyLevel, x_offset, y_offset, width, height);
                }
            }

            if (ch == UP || ch == DOWN || ch == LEFT || ch == RIGHT) {
                last_direction = ch; 
            }

            if (eat_itself(snake)) {
                lives--;
                if (lives <= 0) {
                    state = DEAD;
                } else {
                    reset_game(state, snake, foods, obstacles, obstacle_count, difficultyLevel, x_offset, y_offset, width, height);
                }
            }

            // avoid snake reversing direction
            if ((last_direction == UP && prevDirection == DOWN) ||
                (last_direction == DOWN && prevDirection == UP) ||
                (last_direction == LEFT && prevDirection == RIGHT) ||
                (last_direction == RIGHT && prevDirection == LEFT)) {
                last_direction = prevDirection;
            }

            if (food_exists(foods, snake->x, snake->y)) {
                enum Type foodType = food_type(foods, snake->x, snake->y);

                std::string foodTypeStr = (foodType == Increase) ? "Increase" : "Decrease";
                
                if (foodType == Increase) {
                     // Increase snake size
                    foods = remove_eaten_food(foods, snake->x, snake->y);
                    
                    snake = move_snake(snake, last_direction, 1);
                    score += 20;

                    Food* new_food = create_food(rand() % (width - 2) + x_offset + 1, rand() % (height - 2) + y_offset + 1, static_cast<Type>(rand() % 4));                    add_new_food(foods, new_food);

                } else if (foodType == Decrease) {

                    foods = remove_eaten_food(foods, snake->x, snake->y);

                    const int shrink_by = -1;

                    // min snake length is 1
                    if (len(snake) + shrink_by <= 0) {
                        lives--;
                        if (lives <= 0) {
                            state = DEAD;
                        } else {
                            reset_game(state, snake, foods, obstacles, obstacle_count, difficultyLevel, x_offset, y_offset, width, height);
                        }
                        continue;
                    }

                    snake = move_snake(snake, last_direction, shrink_by); 

                    Food* new_food = create_food(rand() % (width - 2) + x_offset + 1, rand() % (height - 2) + y_offset + 1, static_cast<Type>(rand() % 4));                    add_new_food(foods, new_food);

                    if (score - 10 >= 0) {
                        score -= 10;
                    } else {
                        lives--;
                        if (lives <= 0) {
                            state = DEAD;
                            break;
                        } else {
                            reset_game(state, snake, foods, obstacles, obstacle_count, difficultyLevel, x_offset, y_offset, width, height);
                        }
                    }

                } else if (foodType == IncreaseMore) {
                    foods = remove_eaten_food(foods, snake->x, snake->y);
                    snake = move_snake(snake, last_direction, 2);
                    score += 40;

                    Food* new_food = create_food(rand() % (width - 2) + x_offset + 1, rand() % (height - 2) + y_offset + 1, static_cast<Type>(rand() % 4));                    add_new_food(foods, new_food);

                } else if (foodType == DecreaseMore) {
                    foods = remove_eaten_food(foods, snake->x, snake->y);

                    const int shrink_by = -2;

                    // min snake length is 1
                    if (len(snake) + shrink_by <= 0) {
                        lives--;
                        if (lives <= 0) {
                            state = DEAD;
                        } else {
                            reset_game(state, snake, foods, obstacles, obstacle_count, difficultyLevel, x_offset, y_offset, width, height);
                        }
                        continue;
                    }

                    snake = move_snake(snake, last_direction, shrink_by);

                    if (score - 20 >= 0) {
                        score -= 20;
                    } else {
                        lives--;
                        if (lives <= 0) {
                            state = DEAD;
                        } else {
                            reset_game(state, snake, foods, obstacles, obstacle_count, difficultyLevel, x_offset, y_offset, width, height);
                        }
                    }

                    Food* new_food = create_food(rand() % (width - 2) + x_offset + 1, rand() % (height - 2) + y_offset + 1, static_cast<Type>(rand() % 4));                    add_new_food(foods, new_food);
                }

            } else {
                snake = move_snake(snake, last_direction, 0); // snake didnt eat
            }



            // Draw everything on the screen
            clear();
            draw_Gamewindow(window);

            mvprintw(20, 20, "Score: %d", score);
            mvprintw(22, 20, "Lives: %d", lives); 
            mvprintw(24, 20, "Snake coords: %d %d", snake->x, snake->y);
            mvprintw(26, 20, "Speed multiplier: %.2f", speedMultiplier);

            
            draw_snake(snake);
            draw_food(foods);
            draw_obstacles(obstacles);
            
            break;
        }

        case DEAD:
            clear();

            update_best_scores("saves/save_best_10.game", score);
            attron(COLOR_PAIR(1)); 
            mvprintw(10, 10, "GAME OVER");
            attroff(COLOR_PAIR(1)); 
            mvprintw(12, 10, "Final Score: %d", score);
            display_best_scores("saves/save_best_10.game");
            mvprintw(14, 10, "Press 'q' to quit or 'r' to restart.");
            refresh();
            while (true) {
                ch = get_char();
                if (ch == 'q' || ch == 'Q') {
                    endwin();
                    exit(1);
                    break;
                } else if (ch == 'r' || ch == 'R') {
                    endwin();
                    // restart game binary. this effectively resets the snake speed as required
                    std::system("./snake_game"); 
                    exit(0);
                }
            }
            break;

        case PAUSED:
            mvprintw(30, 20, "GAME PAUSED");
            if (ch == 'p' || ch == 'P') {
                state = ALIVE;
            }
            break;

        case EXIT:
            endwin();
            exit(1);
            break;
        default:
            break;
        }

        refresh();
        nanosleep(&timeret, NULL);
    }
}

void reset_game(State& state, Snake*& snake, Food*& foods, Obstacle*& obstacles, int& obstacle_count, int difficultyLevel, int x_offset, int y_offset, int width, int height) {
    state = ALIVE; // Assuming ALIVE is a valid enumerator in the State enum
    snake = init_snake(x_offset + (width / 2), y_offset + (height / 2));
    generate_initial_food(foods, width - 2, height - 2, x_offset + 1, y_offset + 1, difficultyLevel); // avoid spawning food on the border
    
    obstacle_count = rand() % 8 + 3; // random num between 3 and 10
    switch(difficultyLevel) {
        case 1: // Hard
            obstacle_count *= 2;
            break;
        case 2: // Advanced
            obstacle_count *= 4;
            break;
        default:
            break;
    }

    obstacles = NULL;
    generate_obstacles(obstacles, obstacle_count, width, height, x_offset, y_offset);
}

void display_best_scores(const std::string& filename) {
    std::vector<int> scores;
    std::ifstream inFile(filename);
    int score;

    while (inFile >> score) {
        scores.push_back(score);
    }
    inFile.close();

    std::sort(scores.rbegin(), scores.rend());

    mvprintw(16, 10, "Best Scores:");
    int yPos = 18;
    for(size_t i = 0; i < std::min(scores.size(), static_cast<size_t>(10)); ++i) {
        mvprintw(yPos, 10, "%d. %d", i + 1, scores[i]);
        yPos += 2;
    }
}

void update_best_scores(const std::string& filename, int newScore) {
    std::vector<int> scores;
    std::ifstream inFile(filename);
    int score;

    // read existing scores
    while (inFile >> score) {
        scores.push_back(score);
    }
    inFile.close();

    // add new score and sort
    scores.push_back(newScore);
    std::sort(scores.begin(), scores.end(), std::greater<int>());

    if (scores.size() > 10) {
        scores.resize(10);
    }

    // write updated leaderboard to file
    std::ofstream outFile(filename);
    for (const auto& s : scores) {
        outFile << s << std::endl;
    }
    outFile.close();
}