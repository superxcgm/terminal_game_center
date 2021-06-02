#ifndef XC_LINKLIST

#include <list>
#include "graph.h"

#endif

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
#define SYMBOL_BORDER_ON    '#'
#define SYMBOL_BORDER_OFF    '.'
#define ONDEBUG

struct snake_setting {
    int level;
    int border;
};

class snake {
public:
    /* use Singly Linked List to storage veer points of snake */
    Point head() {
        return data.front();
    }

    Point tail() {
        return data.back();
    }

    std::reverse_iterator<std::list<Point, std::allocator<Point>>::iterator> tailRef() {
        return data.rbegin();
    }

    std::reverse_iterator<std::list<Point, std::allocator<Point>>::iterator> headRef() {
        return data.rend();
    }

    void add_to_head(const Point &p) {
        data.push_front(p);
    }

    void add(const Point& p) {
        data.push_back(p);
    }

    void head_move_left() {
        auto &head = data.front();
        head.x--;
    }

    void head_move_right() {
        auto &head = data.front();
        head.x++;
    }

    void head_move_down() {
        auto &head = data.front();
        head.y++;
    }

    void head_move_up() {
        auto &head = data.front();
        head.y--;
    }

    void update_head_x(int x) {
        auto &head = data.front();
        head.x = x;
    }

    void update_head_y(int y) {
        auto &head = data.front();
        head.y = y;
    }

    void update_head(const Point& p) {
        auto &head = data.front();
        head.x = p.x;
        head.y = p.y;
    }

    int dir;
private:
    std::list<Point> data;
};

void init();

void before_destory();

int draw_menu();

void draw_fruit();

int is_hit_wall();

void print_snake();

int is_hit_body(int flag);

void game_over();

void draw_body_line(const Point *p1, const Point *p2);

void draw_border(char ch);

void draw_control_menu(int flag, int base);

void on_game();

void redraw_snack(int signum);

void load_all_res();