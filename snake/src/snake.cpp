#include <ncurses.h>
#include <string>

#include "snake.h"
#include "log.h"

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

void Snake::update(const Rect &rect) {
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
        if (std::abs(tailRef->x - pre_tail->x) == 1 ||
            std::abs(tailRef->x - pre_tail->x) >= rect.get_width() - 2
                ) {
            data.pop_back();
        } else {
            tailRef->x += pre_tail->x > tailRef->x ? 1 : -1;
        }
    } else if (is_vertical_line(*tailRef, *pre_tail)) {
        // vertical
        if (std::abs(tailRef->y - pre_tail->y) == 1 ||
            std::abs(tailRef->y - pre_tail->y) >= rect.get_height() - 2
                ) {
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
    std::string str_points;
    for (auto &it : data) {
        str_points += ", (" + std::to_string(it.x) + ", " + std::to_string(it.y) + ")";
    }
    std::string msg = "Snake: " + str_points + "\n";
    Log::debug(msg.c_str());
}

void Snake::duplicate_tail() {
    data.push_back(*data.rbegin());
}

bool Snake::is_hit(const Point &p, bool ignore_head, const Rect &rect, bool is_real_wall) {
    auto head = data.begin();
    auto start = data.begin();
    if (ignore_head) {
        while (head->x == start->x && head->y == start->y)
            start++;
    }

    for (auto it = start;; it++) {
        auto next = std::next(it);
        if (next == data.end()) {
            break;
        }

        if (!is_real_wall) {
            if (std::abs(it->x - next->x) >= rect.get_width() - 2 ||
                std::abs(it->y - next->y) >= rect.get_height() - 2
                    ) {
                continue;
            }
        }
        if (is_horizontal_line(*it, *next) && it->y == p.y) {
            int min_x = std::min(it->x, next->x);
            int max_x = std::max(it->x, next->x);
            if (min_x <= p.x && p.x <= max_x) {
                Log::debug("Hit, min_x: %d, max_x: %d, p_x: %d\n", min_x, max_x, p.x);
                return true;
            }
        } else if (is_vertical_line(*it, *next) && it->x == p.x) {
            int min_y = std::min(it->y, next->y);
            int max_y = std::max(it->y, next->y);
            if (min_y <= p.y && p.y <= max_y) {
                Log::debug("Hit, min_y: %d, max_y: %d, p_y: %d\n", min_y, max_y, p.y);
                return true;
            }
        }
    }
    return false;
}

void Snake::add_head(const Point &p) {
    data.push_front(p);
}
