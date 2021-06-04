#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

#include <list>
#include "graph.h"
#include <cstdarg>


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

struct snake_setting {
    int level;
    int border;
};

class Snake {
public:

    Snake() {
        data.push_back({INIT_X, INIT_Y});
        data.push_back({INIT_X - INIT_LEN, INIT_Y});
        direction = DIR_RIGHT;
    }

    void draw() {
        auto head = data.front();
        mvaddch(head.y, head.x, SYMBOL_SNAKE_HEAD);
        for (auto it = data.begin();; it++) {
            auto next = std::next(it);
            if (next == data.end()) {
                break;
            }
            if (is_vertical_line(*it, *next)) {
                mvvline(std::min(it->y, next->y), it->x, SYMBOL_SNAKE_BODY, std::abs(it->y - next->y) + 1);
            } else {
                mvhline(it->y, std::min(it->x, next->x), SYMBOL_SNAKE_BODY, std::abs(it->x - next->x) + 1);
            }
        }
    }

    void update() {
        auto &head = data.front();
        mvaddch(head.y, head.x, SYMBOL_SNAKE_BODY);

        switch (direction) {
            case DIR_LEFT:
                head.x--;
                break;
            case DIR_RIGHT:
                head.x++;
                break;
            case DIR_DOWN:
                head.y++;
                break;
            case DIR_UP:
                head.y--;
                break;
        }
        mvaddch(head.y, head.x, SYMBOL_SNAKE_HEAD);

        auto tailRef = data.rbegin();
        auto pre_tail = std::next(tailRef);
        mvaddch(tailRef->y, tailRef->x, SYMBOL_BLANK);

        if (is_hit_wall()) {
            // todo:
        }

//        print_snake();
        if (is_horizontal_line(*tailRef, *pre_tail)) {
            // todo: border and refactor
            if (std::abs(tailRef->x - pre_tail->x) == 1 ||
                (tailRef->x == WIN_COLS - 2 && pre_tail->x == 1) ||
                (tailRef->x == 1 && pre_tail->x == WIN_COLS - 2)) {
                data.pop_back();
            } else {
                tailRef->x += pre_tail->x > tailRef->x ? 1 : -1;
            }
        } else if (is_vertical_line(*tailRef, *pre_tail)) {
            // vertical
            if (std::abs(tailRef->y - pre_tail->y) == 1) {
                data.pop_back();
            } else {
                tailRef->y += pre_tail->y > tailRef->y ? 1 : -1;
            }
        }
    }

    void change_direction(int new_direction) {
        if (!valid_change_direction(new_direction))
            return;
        if (direction == new_direction)
            return;
        direction = new_direction;
        data.push_front(data.front());
    }

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
    bool valid_change_direction(int new_direction) {
        return !((direction == DIR_LEFT && new_direction == DIR_RIGHT) ||
                 (direction == DIR_RIGHT && new_direction == DIR_LEFT) ||
                 (direction == DIR_UP && new_direction == DIR_DOWN) ||
                 (direction == DIR_DOWN && new_direction == DIR_UP));
    }

    void print_snake() {
        fprintf(stderr, "Snake:\n");
        for (auto &it : data) {
            fprintf(stderr, "(%d, %d)\n", it.x, it.y);
        }
    }

    bool is_hit_wall() {
        return head().x == 0
               || head().y == 0
               || head().x == WIN_COLS - 1
               || head().y == WIN_LINES - 1;
    }

    std::list<Point> data;
};

void init();

void before_destory();

int draw_menu();

void draw_fruit();

int is_hit_body(int flag);

void game_over();

void draw_border(char ch);

void draw_control_menu(int flag, int base);

void on_game();

void redraw_snack(int signum);

void load_all_res();


#endif