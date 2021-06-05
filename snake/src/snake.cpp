#include <ncurses.h>

#include "snake.h"

Snake::Snake() {
    data.push_back({INIT_X, INIT_Y});
    data.push_back({INIT_X - INIT_LEN, INIT_Y});
    direction = DIR_RIGHT;
}

void Snake::draw() {
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

void Snake::update() {
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

//        print_snake();
    if (is_horizontal_line(*tailRef, *pre_tail)) {
        // todo: border and refactor
        if (std::abs(tailRef->x - pre_tail->x) == 1) {
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

void Snake::change_direction(int new_direction) {
    if (!valid_change_direction(new_direction))
        return;
    if (direction == new_direction)
        return;
    direction = new_direction;
    data.push_front(data.front());
}

bool Snake::valid_change_direction(int new_direction) {
    return !((direction == DIR_LEFT && new_direction == DIR_RIGHT) ||
             (direction == DIR_RIGHT && new_direction == DIR_LEFT) ||
             (direction == DIR_UP && new_direction == DIR_DOWN) ||
             (direction == DIR_DOWN && new_direction == DIR_UP));
}

void Snake::print_snake() {
    fprintf(stderr, "Snake:\n");
    for (auto &it : data) {
        fprintf(stderr, "(%d, %d)\n", it.x, it.y);
    }
}
