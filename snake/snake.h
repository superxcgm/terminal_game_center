#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

#include <list>
#include "graph.h"

#define WIN_LINES            24
#define WIN_COLS            80
#define VERSION                "0.5"
#define BORDER_ON            1
#define BORDER_OFF            0
#define INIT_X                40
#define INIT_Y                12
#define INIT_LEN            3
#define DIR_LEFT            1
#define DIR_RIGHT            2
#define DIR_UP                3
#define DIR_DOWN            4
#define SYMBOL_SNAKE_BODY    'o'
#define SYMBOL_SNAKE_HEAD    '@'
#define SYMBOL_FRUIT        '$'
#define SYMBOL_BLANK        ' '
#define ONDEBUG

class Snake {
public:

    Snake();

    void draw();

    void update();

    void change_direction(int new_direction);

    Point head() {
        return data.front();
    }

    std::reverse_iterator<std::list<Point, std::allocator<Point>>::iterator> tailRef() {
        return data.rbegin();
    }

    std::reverse_iterator<std::list<Point, std::allocator<Point>>::iterator> headRef() {
        return data.rend();
    }

    int direction;
private:
    bool valid_change_direction(int new_direction);

    void print_snake();

    bool is_hit_wall();

    std::list<Point> data;
};

#endif