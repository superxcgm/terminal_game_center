#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <queue>
#include "res.h"
#include "config.h"
#include "snake.h"
#include "menu.h"

class Game {
public:
    Game();

    void init();

    void run();

    void destroy();

private:

    Snake the_snake;

    bool is_game_over;

    std::queue<int> queue_dir;

    Rect rect;

    Point fruit;

    Menu menu;

    Config config;

    Resource res_game_over;

    bool is_hit_wall();

    void on_game();

    void add_border_back();

    void game_over();

    void draw_fruit();

    bool is_hit_body();

    bool is_hit_fruit();

    bool is_hit_snake(const Point &point);

    friend void redraw_snack(int signum);
};


#endif //SNAKE_GAME_H
