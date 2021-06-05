#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

#include <list>
#include "graph.h"

#define WIN_LINES            24
#define WIN_COLS            80
#define VERSION                "0.5"
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

class Snake {
public:

    Snake();

    void draw();

    void update(const Rect& rect);

    void add_head(const Point& p);

    void change_direction(int new_direction);

    void duplicate_tail();

    bool is_hit(const Point& p, bool ignore_head, const Rect &rect, bool is_real_wall);

    Point head() {
        return data.front();
    }

    int direction;
private:
    bool valid_change_direction(int new_direction);

    void print_snake();

    std::list<Point> data;

    friend void redraw_snack(int signum);
};

#endif