/*
 * snake.c
 * @author superxc
 * e-mail: superxc@outlook.com
 * 20170710
 * v0.5
 */
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include "snake.h"
#include "res.h"
#include "xc_queue.h"

struct snake_setting setting;
struct snake the_snake;
int score;
int gover;
struct xc_queue queue_dir;
struct xc_point fruit;

struct XC_Res *res_snake;
struct XC_Res *res_game_over;
struct XC_Res *res_control_menu;

int main(void) {
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
    srand(time(0));
    cbreak(); /* donot buffer input */
    noecho();
    curs_set(0); /* donot display cursor */
    keypad(stdscr, TRUE); /* enable arrow key */
    /* ignore INT and QUIT signals */
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void load_all_res() {

    res_snake = load_res("res/snake.res");
    res_control_menu = load_res("res/control_menu.res");
    res_game_over = load_res("res/game_over.res");
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

    for (i = 0; i < res_snake->cnt; ++i)
        mvaddstr(i + 3, 3, res_snake->data[i]);
    addstr("   V"VERSION); /* strcat */
    draw_border('*');
    control_menu_base = 3 + res_snake->cnt + 3;
    setting.level = 1;
    setting.border = BORDER_ON;
    draw_control_menu(0, control_menu_base); /* 0=>base, 1=>control part */
    draw_control_menu(1, control_menu_base);
    refresh();
    while (1) {
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
                break;
            case ' ':
            case '\n':
                on_game();
                return 1;    /* reenter menu */
                break;
        }
    }
}

void on_game() {
    int set_ticker(int n_msecs);
    int ch;
    int pre_ch;
    int delay;
    score = 0;
    gover = 1;
    if (xc_queue_init(&queue_dir) == 0) {
        before_destory();
        printf("Can not initialize queue_dir.\n");
        exit(1);
    }
    clear(); /* clear screen */
    draw_border(setting.border == BORDER_ON ?
                SYMBOL_BORDER_ON : SYMBOL_BORDER_OFF);
    /* init snake */
    the_snake.head = (struct node_front *) malloc(sizeof(struct node_front));
    the_snake.head->pos.x = INIT_X;
    the_snake.head->pos.y = INIT_Y;
    the_snake.head->prev = NULL;
    the_snake.tail = (struct node_front *) malloc(sizeof(struct node_front));
    the_snake.tail->pos.y = the_snake.head->pos.y;
    the_snake.tail->pos.x = the_snake.head->pos.x - INIT_LEN;
    the_snake.tail->prev = the_snake.head;
    the_snake.dir = DIR_RIGHT;

    /* draw snake */
    draw_body_line(&the_snake.head->pos, &the_snake.tail->pos);
    mvaddch(the_snake.head->pos.y, the_snake.head->pos.x, SYMBOL_SNAKE_HEAD);

    draw_fruit();

    signal(SIGALRM, redraw_snack);
    delay = 20 * (10 - setting.level);
    set_ticker(delay);    /* speed of snake would not change during play */
    pre_ch = ' ';
    while (1) {
        ch = getch();
        if (ch == pre_ch)    /* ignore duplication key press */
            continue;
        if (gover)
            switch (ch) {
                case 'm':
                case 'M':
                    return;    /* back to menu */
                    break;
                case '\n':
                    on_game();    /* just recursion */
                    return;
                    break;
            }
        switch (ch) {
            case 'w':
            case 'W':
            case KEY_UP:
                /* use queue to buffer direction change request, if change
                the_snake.direction directly, later request will cover the
                previous request. */
                if (pre_ch == KEY_DOWN) continue; /* forbid reverse */
                xc_queue_insert(&queue_dir, DIR_UP);
                pre_ch = KEY_UP;
                break;
            case 's':
            case 'S':
            case KEY_DOWN:
                if (pre_ch == KEY_UP) continue; /* forbid reverse */
                xc_queue_insert(&queue_dir, DIR_DOWN);
                pre_ch = KEY_DOWN;
                break;
            case 'a':
            case 'A':
            case KEY_LEFT:
                if (pre_ch == KEY_RIGHT) continue; /* forbid reverse */
                xc_queue_insert(&queue_dir, DIR_LEFT);
                pre_ch = KEY_LEFT;
                break;
            case 'd':
            case 'D':
            case KEY_RIGHT:
                if (pre_ch == KEY_LEFT) continue; /* forbid reverse */
                xc_queue_insert(&queue_dir, DIR_RIGHT);
                pre_ch = KEY_RIGHT;
                break;
#ifdef ONDEBUG
            case 'q':
            case 'Q':
                before_destory();
                exit(0);
                break;
#endif
        }
    }
}

/* only support horizontail or vertical line */
void draw_body_line(const struct xc_point *p1, const struct xc_point *p2) {
    int dir;
    int i;
    const struct xc_point *tmp_point;
    if (p1->x == p2->x) {    /* vertical */
        /* make sure that p1->y < p2->y*/
        if (p1->y > p2->y) {
            tmp_point = p1;
            p1 = p2;
            p2 = tmp_point;
        }
        for (i = p1->y; i <= p2->y; ++i) {
            move(i, p1->x);
            addch(SYMBOL_SNAKE_BODY);
        }

    } else if (p1->y == p2->y) {    /* horizontal */
        if (p1->x > p2->x) {
            tmp_point = p1;
            p1 = p2;
            p2 = tmp_point;
        }
        move(p1->y, p1->x);
        for (i = p1->x; i <= p2->x; ++i) addch(SYMBOL_SNAKE_BODY);
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
    struct node_front *p;
    struct node_front *prev;
    struct xc_point *des;

    p = the_snake.tail;
    prev = p->prev;
    des = flag ? &the_snake.head->pos : &fruit;
    // if(flag == 1){
    // 	fprintf(stderr, "is_hit_body(1):\n");
    // 	fprintf(stderr, "check point: (%d, %d)\n", des->x, des->y);
    // }
    while (prev) {
        if (flag && prev->pos.x == the_snake.head->pos.x &&
            prev->pos.y == the_snake.head->pos.y)
            break;
        if (setting.border == BORDER_OFF) {
            if ((p->pos.x == WIN_COLS - 2 && prev->pos.x == 1) ||
                (p->pos.x == 1 && prev->pos.x == WIN_COLS - 2) ||
                (p->pos.y == WIN_LINES - 2 && prev->pos.y == 1) ||
                (p->pos.y == 1 && prev->pos.y == WIN_LINES - 2)) {
                p = prev;
                prev = p->prev;
                continue;
            }
        }
        // if(flag == 1){
        // 	fprintf(stderr, "check point is on segment (%d, %d) - (%d, %d)\n", p->pos.x, p->pos.y, prev->pos.x, prev->pos.y);
        // }
        if (p->pos.x == prev->pos.x && p->pos.x == des->x) {
            if ((des->y >= p->pos.y && des->y <= prev->pos.y) ||
                (des->y >= prev->pos.y && des->y <= p->pos.y))
                return 1;
        } else if (p->pos.y == prev->pos.y && p->pos.y == des->y) {
            if ((des->x >= p->pos.x && des->x <= prev->pos.x) ||
                (des->x >= prev->pos.x && des->x <= p->pos.x))
                return 1;
        }
        p = prev;
        prev = p->prev;
        /* head node check do not check first segment */
    }
    return 0;
}

void redraw_snack(int signum) {
    int new_dir;
    // int has_veer = 0;
    struct node_front *prev;
    struct timeval tv1;
    struct timeval tv2;
    struct xc_point prev_pos;

    gettimeofday(&tv1, NULL);
    /* need veer */
    if (xc_queue_get(&queue_dir, &new_dir) &&
        the_snake.dir != new_dir &&
        !((the_snake.dir == DIR_LEFT && new_dir == DIR_RIGHT) ||
          (the_snake.dir == DIR_RIGHT && new_dir == DIR_LEFT) ||
          (the_snake.dir == DIR_UP && new_dir == DIR_DOWN) ||
          (the_snake.dir == DIR_DOWN && new_dir == DIR_UP))) {
        struct xc_point pos;

        pos = the_snake.head->pos;
        the_snake.dir = new_dir;
        the_snake.head = add_to_head(the_snake.head, &pos);
        // has_veer = 1;
    }
    prev_pos = the_snake.head->pos;
    /* head node step forward */
    mvaddch(the_snake.head->pos.y, the_snake.head->pos.x, SYMBOL_SNAKE_BODY);
    switch (the_snake.dir) {
        case DIR_LEFT:
            --the_snake.head->pos.x;
            break;
        case DIR_RIGHT:
            ++the_snake.head->pos.x;
            break;
        case DIR_DOWN:
            ++the_snake.head->pos.y;
            break;
        case DIR_UP:
            --the_snake.head->pos.y;
            break;
    }
    mvaddch(the_snake.head->pos.y, the_snake.head->pos.x, SYMBOL_SNAKE_HEAD);
    /* hit wall  or hit body */
    if (is_hit_wall()) {
        if (setting.border == BORDER_ON)
            goto gmover;
        else {
            struct xc_point pos;

            mvaddch(the_snake.head->pos.y, the_snake.head->pos.x,
                    SYMBOL_BORDER_OFF);

            if (the_snake.head->pos.x == WIN_COLS - 1)
                the_snake.head->pos.x = 1;
            if (the_snake.head->pos.x == 0)
                the_snake.head->pos.x = WIN_COLS - 2;

            if (the_snake.head->pos.y == WIN_LINES - 1)
                the_snake.head->pos.y = 1;
            if (the_snake.head->pos.y == 0)
                the_snake.head->pos.y = WIN_LINES - 2;

            pos = the_snake.head->pos;        /* other side of border */
            // fprintf(stderr, "Points in other side: (%d, %d)\n", pos.x, pos.y);
            the_snake.head->pos = prev_pos;    /* this side of border */
            // fprintf(stderr, "Points in this side: (%d, %d)\n", prev_pos.x, prev_pos.y);
            /* generate two points to across a border */
            the_snake.head = add_to_head(the_snake.head, &pos);
            the_snake.head = add_to_head(the_snake.head, &pos);
        }
    }
    if (is_hit_body(1)) goto gmover;

    /* eat fruit */
    if (the_snake.head->pos.x == fruit.x && the_snake.head->pos.y == fruit.y) {
        draw_fruit();
        /* when eat a fruit, the length of snake add one, so tail do not move
         forward */
        return;
    }
    /* last node step forward */
    mvaddch(the_snake.tail->pos.y, the_snake.tail->pos.x, SYMBOL_BLANK);
    prev = the_snake.tail->prev;

    if (prev->pos.y == the_snake.tail->pos.y) {
        /* dis == 1 or on the both side of the border */
        if (abs(the_snake.tail->pos.x - prev->pos.x) == 1 ||
            (the_snake.tail->pos.x == WIN_COLS - 2 && prev->pos.x == 1) ||
            (the_snake.tail->pos.x == 1 && prev->pos.x == WIN_COLS - 2)) {
            // fprintf(stderr, "free point: (%d, %d)\n", the_snake.tail->pos.x, the_snake.tail->pos.y);
            // fprintf(stderr, "previous node: (%d, %d)\n", prev->pos.x, prev->pos.y);
            free(the_snake.tail);
            the_snake.tail = prev;
        } else
            the_snake.tail->pos.x += prev->pos.x > the_snake.tail->pos.x ?
                                     1 : -1;


    } else if (prev->pos.x == the_snake.tail->pos.x) {
        if (abs(the_snake.tail->pos.y - prev->pos.y) == 1 ||
            (the_snake.tail->pos.y == WIN_LINES - 2 && prev->pos.y == 1) ||
            (the_snake.tail->pos.y == 1 && prev->pos.y == WIN_LINES - 2)) {
            free(the_snake.tail);
            the_snake.tail = prev;
        } else
            the_snake.tail->pos.y += prev->pos.y > the_snake.tail->pos.y ?
                                     1 : -1;
    }
    refresh();
    gettimeofday(&tv2, NULL);
    // fprintf(stderr, "this signal handle take %f ms.\n", tv2.tv_sec * 1000 + tv2.tv_usec / 1000.0 - (tv1.tv_sec * 1000 + tv1.tv_usec / 1000.0));
    /* less than 1ms on my machine */
    // print_snake();
    return;
    gmover:
    attron(A_BOLD);
    mvaddch(the_snake.head->pos.y, the_snake.head->pos.x, 'x');
    attroff(A_BOLD);
    game_over();
}

void print_snake() {
    struct node_front *prev;
    prev = the_snake.tail;
    fprintf(stderr, "snake:\n");
    while (prev) {
        fprintf(stderr, "(%d, %d)\n", prev->pos.x, prev->pos.y);
        prev = prev->prev;
    }
}

int is_hit_wall() {
    if (the_snake.head->pos.x == 0) return 1;
    if (the_snake.head->pos.y == 0) return 1;
    if (the_snake.head->pos.x == WIN_COLS - 1) return 1;
    if (the_snake.head->pos.y == WIN_LINES - 1) return 1;
    return 0;
}

void game_over() {
    int set_ticker(int n_msecs);
    int i;
    int left_offset;

    signal(SIGALRM, SIG_IGN);
    set_ticker(0); /* do not redraw snake any more */

    left_offset = (WIN_COLS - strlen(res_game_over->data[0])) / 2;
    for (i = 0; i < res_game_over->cnt; ++i)
        mvaddstr(3 + i, left_offset, res_game_over->data[i]);
    gover = 1;
    /* free storage */
    free_node_front(the_snake.tail);
    refresh();
}

void draw_control_menu(int flag, int base) {
    const int left_offset = 7;
    const int height = 10;
    const int width = 53;
    int i;
    /* 0=>base, 1=>control part */
    switch (flag) {
        case 0:

            for (i = 0; i < res_control_menu->cnt; ++i)
                mvaddstr(base + i, left_offset, res_control_menu->data[i]);
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