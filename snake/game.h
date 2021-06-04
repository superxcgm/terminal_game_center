#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <queue>
#include "res.h"
#include "config.h"
#include "snake.h"

class Game {
public:
    Game();

    void init();

    void run();

    void destroy();

private:
    bool draw_menu();

    // todo: use smart pointer to fix memory leak
    Resource *res_snake;
    Resource *res_control_menu;
    Resource *res_game_over;

    Snake the_snake;

    int gover = 1;

    std::queue<int> queue_dir;

    Rect rect;

    Point fruit;

    void load_all_res();

    void on_game(const Config& config);

    void game_over();

    void draw_fruit();

    int is_hit_body(int flag);

    void draw_control_menu(int flag, int base, const Config& config);

    friend void redraw_snack(int signum);
};


#endif //SNAKE_GAME_H
