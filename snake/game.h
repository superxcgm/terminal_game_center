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

    int gover = 1;

    std::queue<int> queue_dir;

    Rect rect;

    Point fruit;

    Menu menu;

    Resource res_game_over;

    void on_game(const Config& config);

    void game_over();

    void draw_fruit();

    int is_hit_body(int flag);

    friend void redraw_snack(int signum);
};


#endif //SNAKE_GAME_H
