/*
 * Snake.c
 * @author superxc
 * e-mail: superxc@outlook.com
 * 20170710
 * v0.5
 */
#include <cstdio>
#include <cstdlib>
#include <curses.h>
#include <cstring>
#include <csignal>
#include <sys/time.h>
#include <ctime>
#include "snake.h"
#include "res.h"
#include <queue>

struct snake_setting setting;
Snake the_snake;
int gover;
std::queue<int> queue_dir;
Point fruit;

Resource *res_snake;
Resource *res_game_over;
Resource *res_control_menu;

int main(int argc, char **argv) {
    init();
    while (draw_menu());
    before_destory();
    return 0;
}

void init() {
    initscr();
    if (COLS < WIN_COLS || LINES < WIN_LINES) {
        endwin();
        fprintf(stderr, "Your console screen is smaller than %dx%d\n",
                WIN_COLS, WIN_LINES);
        fprintf(stderr, "Please resize your window and try again\n");
        exit(1);
    }
    load_all_res();
    srand(time(nullptr));
    cbreak(); /* donot buffer input */
    noecho();
    curs_set(0); /* donot display cursor */
    keypad(stdscr, TRUE); /* enable arrow key */
    /* ignore INT and QUIT signals */
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void load_all_res() {

    // Todo: Remove new here
    res_snake = new Resource("res/Snake.res");
    res_control_menu = new Resource("res/control_menu.res");
    res_game_over = new Resource("res/game_over.res");
}

void before_destory() {
    curs_set(1); /* display cursor */
    endwin();
}

int draw_menu() {
    int i;
    int ch;
    int control_menu_base;

    clear();

    // Todo: remove this logic to res or some display layer
    for (i = 0; i < res_snake->line_count(); ++i)
        mvaddstr(i + 3, 3, res_snake->get_line(i).c_str());

    addstr("   V" VERSION); /* strcat */
    draw_border('*');
    control_menu_base = 3 + res_snake->line_count() + 3;
    setting.level = 1;
    setting.border = BORDER_ON;
    draw_control_menu(0, control_menu_base); /* 0=>base, 1=>control part */
    draw_control_menu(1, control_menu_base);
    refresh();
    while (true) {
        ch = getch();
        switch (ch) {
            case KEY_LEFT:
                if (setting.level > 1) {
                    --setting.level;
                    draw_control_menu(1, control_menu_base);
                }
                break;
            case KEY_RIGHT:
                if (setting.level < 9) {
                    ++setting.level;
                    draw_control_menu(1, control_menu_base);
                }
                break;
            case KEY_UP:
                if (setting.border == BORDER_OFF) {
                    setting.border = BORDER_ON;
                    draw_control_menu(1, control_menu_base);
                }
                break;
            case KEY_DOWN:
                if (setting.border == BORDER_ON) {
                    setting.border = BORDER_OFF;
                    draw_control_menu(1, control_menu_base);
                }
                break;
            case 'q': /* quit */
            case 'Q':
                return 0;
            case ' ':
            case '\n':
                on_game();
                return 1;    /* reenter menu */
            default:
                break;
        }
    }
}

void on_game() {
    int set_ticker(int n_msecs);
    int ch;
    int pre_ch;
    int delay;
    gover = 1;
    clear(); /* clear screen */
    draw_border(setting.border == BORDER_ON ?
                SYMBOL_BORDER_ON : SYMBOL_BORDER_OFF);

    /* draw Snake */
    the_snake.draw();

    draw_fruit();

    signal(SIGALRM, redraw_snack);
    delay = 20 * (10 - setting.level);
    set_ticker(delay);    /* speed of Snake would not change during play */
    pre_ch = ' ';
    while (true) {
        ch = getch();
        if (ch == pre_ch)    /* ignore duplication key press */
            continue;
        pre_ch = ch;
        if (gover)
            switch (ch) {
                case 'm':
                case 'M':
                    return;    /* back to menu */
                case '\n':
                    on_game();    /* just recursion */
                    return;
                default:
                    break;
            }
        switch (ch) {
            case 'w':
            case 'W':
            case KEY_UP:
                /* use queue to buffer direction change request, if change
                the_snake.direction directly, later request will cover the
                previous request. */
                queue_dir.push(DIR_UP);
                break;
            case 's':
            case 'S':
            case KEY_DOWN:
                queue_dir.push(DIR_DOWN);
                break;
            case 'a':
            case 'A':
            case KEY_LEFT:
                queue_dir.push(DIR_LEFT);
                break;
            case 'd':
            case 'D':
            case KEY_RIGHT:
                queue_dir.push(DIR_RIGHT);
                break;
#ifdef ONDEBUG
            case 'q':
            case 'Q':
                before_destory();
                exit(0);
#endif
            default:
                break;
        }
    }
}

void draw_fruit() {
    /* border left:0, border right: WIN_COLS - 1
        fruit shouldn't in the border */
    do {
        fruit.x = rand() % (WIN_COLS - 2) + 1;
        fruit.y = rand() % (WIN_LINES - 2) + 1;
    } while (is_hit_body(0));
    mvaddch(fruit.y, fruit.x, SYMBOL_FRUIT);
    refresh();
}

/* flag:
		0=>fruit check
		1=>head node check
*/
int is_hit_body(int flag) {
//    struct node_front *p;
    Point *des;

    auto p = the_snake.tailRef();
    auto prev = std::prev(p);
    auto head = the_snake.head();
    auto headRef = the_snake.headRef();
    des = flag ? &head : &fruit;
    // if(flag == 1){
    // 	fprintf(stderr, "is_hit_body(1):\n");
    // 	fprintf(stderr, "check point: (%d, %d)\n", des->x, des->y);
    // }
    while (prev != headRef) {
        if (flag && prev->x == head.x &&
            prev->y == head.y)
            break;
        if (setting.border == BORDER_OFF) {
            if ((p->x == WIN_COLS - 2 && prev->x == 1) ||
                (p->x == 1 && prev->x == WIN_COLS - 2) ||
                (p->y == WIN_LINES - 2 && prev->y == 1) ||
                (p->y == 1 && prev->y == WIN_LINES - 2)) {
                p = prev;
                prev = std::prev(p);
                continue;
            }
        }
        // if(flag == 1){
        // 	fprintf(stderr, "check point is on segment (%d, %d) - (%d, %d)\n", p->pos.x, p->pos.y, prev->pos.x, prev->pos.y);
        // }
        if (p->x == prev->x && p->x == des->x) {
            if ((des->y >= p->y && des->y <= prev->y) ||
                (des->y >= prev->y && des->y <= p->y))
                return 1;
        } else if (p->y == prev->y && p->y == des->y) {
            if ((des->x >= p->x && des->x <= prev->x) ||
                (des->x >= prev->x && des->x <= p->x))
                return 1;
        }
        p = prev;
        prev = std::prev(p);
        /* head node check do not check first segment */
    }
    return 0;
}

void redraw_snack(int signum) {
    // int has_veer = 0;
    struct timeval tv1;
    struct timeval tv2;
//    Point prev_pos{0, 0};

    gettimeofday(&tv1, nullptr);
    /* need veer */
    if (!queue_dir.empty()) {
        int new_direction = queue_dir.front();
        queue_dir.pop();
        the_snake.change_direction(new_direction);
    }
    the_snake.update();
//    prev_pos = the_snake.head();
    /* hit wall  or hit body */
//    if (is_hit_wall()) {
//        if (setting.border == BORDER_ON)
//            goto gmover;
//        else {
//            Point pos;
//
//            mvaddch(the_snake.head().y, the_snake.head().x,
//                    SYMBOL_BORDER_OFF);
//
//            if (the_snake.head().x == WIN_COLS - 1)
//                the_snake.update_head_x(1);
//            if (the_snake.head().x == 0)
//                the_snake.update_head_x(WIN_COLS - 2);
//
//            if (the_snake.head().y == WIN_LINES - 1)
//                the_snake.update_head_y(1);
//            if (the_snake.head().y == 0)
//                the_snake.update_head_y(WIN_LINES - 2);
//
//            pos = the_snake.head();        /* other side of border */
//            // fprintf(stderr, "Points in other side: (%d, %d)\n", pos.x, pos.y);
//            the_snake.update_head(prev_pos);    /* this side of border */
//            // fprintf(stderr, "Points in this side: (%d, %d)\n", prev_pos.x, prev_pos.y);
//            /* generate two points to across a border */
//            the_snake.add_to_head(pos);
//            the_snake.add_to_head(pos);
//        }
//    }
//    if (is_hit_body(1)) goto gmover;

    /* eat fruit */
//    if (the_snake.head().x == fruit.x && the_snake.head().y == fruit.y) {
//        draw_fruit();
//        /* when eat a fruit, the length of Snake add one, so tail do not move
//         forward */
//        return;
//    }
    /* last node step forward */
//    mvaddch(the_snake.tail().y, the_snake.tail().x, SYMBOL_BLANK);
//    auto prev = std::prev(the_snake.tailRef());

//    if (prev->y == the_snake.tail().y) {
//        /* dis == 1 or on the both side of the border */
//        if (abs(the_snake.tail().x - prev->x) == 1 ||
//            (the_snake.tail().x == WIN_COLS - 2 && prev->x == 1) ||
//            (the_snake.tail().x == 1 && prev->x == WIN_COLS - 2)) {
//            // fprintf(stderr, "free point: (%d, %d)\n", the_snake.tail->pos.x, the_snake.tail->pos.y);
//            // fprintf(stderr, "previous node: (%d, %d)\n", prev->pos.x, prev->pos.y);
////            free(the_snake.tail);
//            the_snake.tail = prev;
//        } else
//            the_snake.tail->pos.x += prev->pos.x > the_snake.tail->pos.x ?
//                                     1 : -1;
//
//
//    } else if (prev->pos.x == the_snake.tail->pos.x) {
//        if (abs(the_snake.tail->pos.y - prev->pos.y) == 1 ||
//            (the_snake.tail->pos.y == WIN_LINES - 2 && prev->pos.y == 1) ||
//            (the_snake.tail->pos.y == 1 && prev->pos.y == WIN_LINES - 2)) {
//            free(the_snake.tail);
//            the_snake.tail = prev;
//        } else
//            the_snake.tail->pos.y += prev->pos.y > the_snake.tail->pos.y ?
//                                     1 : -1;
//    }
    refresh();
    gettimeofday(&tv2, nullptr);
    // fprintf(stderr, "this signal handle take %f ms.\n", tv2.tv_sec * 1000 + tv2.tv_usec / 1000.0 - (tv1.tv_sec * 1000 + tv1.tv_usec / 1000.0));
    /* less than 1ms on my machine */
    // print_snake();
    return;
    gmover:
    attron(A_BOLD);
    mvaddch(the_snake.head().y, the_snake.head().x, 'x');
    attroff(A_BOLD);
    game_over();
}

void print_snake() {
//    struct node_front *prev;
//    prev = the_snake.tail;
//    fprintf(stderr, "Snake:\n");
//    while (prev) {
//        fprintf(stderr, "(%d, %d)\n", prev->pos.x, prev->pos.y);
//        prev = prev->prev;
//    }
}

int is_hit_wall() {
    if (the_snake.head().x == 0) return 1;
    if (the_snake.head().y == 0) return 1;
    if (the_snake.head().x == WIN_COLS - 1) return 1;
    if (the_snake.head().y == WIN_LINES - 1) return 1;
    return 0;
}

void log(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt != '\0') {
        if (*fmt == 'd') {
            int i = va_arg(args, int);
            std::cout << i << '\n';
        } else if (*fmt == 'c') {
            // note automatic conversion to integral type
            int c = va_arg(args, int);
            std::cout << static_cast<char>(c) << '\n';
        } else if (*fmt == 'f') {
            double d = va_arg(args, double);
            std::cout << d << '\n';
        }
        ++fmt;
    }

    va_end(args);
}

void game_over() {
    int set_ticker(int n_msecs);
    int i;
    int left_offset;

    signal(SIGALRM, SIG_IGN);
    set_ticker(0); /* do not redraw Snake any more */

    left_offset = (WIN_COLS - strlen(res_game_over->get_line(0).c_str())) / 2;
    for (i = 0; i < res_game_over->line_count(); ++i)
        mvaddstr(3 + i, left_offset, res_game_over->get_line(i).c_str());
    gover = 1;
    refresh();
}

void draw_control_menu(int flag, int base) {
    const int left_offset = 7;
    const int width = 53;
    int i;
    /* 0=>base, 1=>control part */
    switch (flag) {
        case 0:

            for (i = 0; i < res_control_menu->line_count(); ++i)
                mvaddstr(base + i, left_offset, res_control_menu->get_line(i).c_str());
            break;
        case 1:
            if (setting.border == BORDER_ON) attron(A_BOLD);
            mvaddstr(base + 5, left_offset + width / 2, "Borders On");
            if (setting.border == BORDER_ON) attroff(A_BOLD);

            if (setting.border == BORDER_OFF) attron(A_BOLD);
            mvaddstr(base + 6, left_offset + width / 2, "Borders Off");
            if (setting.border == BORDER_OFF) attroff(A_BOLD);

            for (i = 1; i <= 9; ++i) {
                if (i == setting.level) attron(A_BOLD);
                mvaddch(base + 8, left_offset + width / 2 + (i - 1) * 2,
                        i + '0');
                if (i == setting.level) attroff(A_BOLD);
            }
            break;
        default:
            break;
    }
    refresh();
}

void draw_border(char ch) {
    move(0, 0);
    hline(ch, WIN_COLS);    /* top */
    move(WIN_LINES - 1, 0);
    hline(ch, WIN_COLS);    /* bottom */
    move(0, 0);
    vline(ch, WIN_LINES);    /* left */
    move(0, WIN_COLS - 1);
    vline(ch, WIN_LINES);    /* right */
}