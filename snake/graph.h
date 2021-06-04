#ifndef SNAKE_GRAPH_H
#define SNAKE_GRAPH_H

#include <ncurses.h>

struct Point {
    int x;
    int y;
};

bool is_horizontal_line(const Point &a, const Point &b);

bool is_vertical_line(const Point &a, const Point &b);

class Rect {
public:
    Rect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}

    void draw(bool is_real) {
        int ch = is_real ? '#' : '.';
        move(0, 0);
        hline(ch, width);
        move(height - 1, 0);
        hline(ch, width);
        move(0, 0);
        vline(ch, height);
        move(0, width - 1);
        vline(ch, height);
    }

private:
    int x;
    int y;
    int width;
    int height;
};

#endif