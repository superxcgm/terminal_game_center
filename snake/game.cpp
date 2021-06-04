#include <ncurses.h>
#include <sys/time.h>
#include <ctime>

#include "game.h"

void Game::init() {
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

void Game::run() {
    while (draw_menu());
}

void Game::destroy() {
    curs_set(1); /* display cursor */
    endwin();
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

bool Game::draw_menu() {
    int i;
    int ch;
    int control_menu_base;

    clear();

    res_snake->draw(3, 3);



    addstr("   V" VERSION); /* strcat */
    draw_border('*');
    control_menu_base = 3 + res_snake->line_count() + 3;
    Config config;
    draw_control_menu(0, control_menu_base, config); /* 0=>base, 1=>control part */
    draw_control_menu(1, control_menu_base, config);
    refresh();
    while (true) {
        ch = getch();
        switch (ch) {
            case KEY_LEFT:
                config.decreaseLevel();
                draw_control_menu(1, control_menu_base, config);
                break;
            case KEY_RIGHT:
                config.increaseLevel();
                draw_control_menu(1, control_menu_base, config);
                break;
            case KEY_UP:
                config.set_real_border(true);
                draw_control_menu(1, control_menu_base, config);
                break;
            case KEY_DOWN:
                config.set_real_border(false);
                draw_control_menu(1, control_menu_base, config);
                break;
            case 'q': /* quit */
            case 'Q':
                return false;
            case ' ':
            case '\n':
                on_game(config);
                return false;    /* reenter menu */
            default:
                break;
        }
    }
}

void Game::load_all_res() {
    res_snake = new Resource("res/Snake.res");
    res_control_menu = new Resource("res/control_menu.res");
    res_game_over = new Resource("res/game_over.res");
}

void Game::draw_control_menu(int flag, int base, const Config& config) {
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
            if (config.is_real_border()) attron(A_BOLD);
            mvaddstr(base + 5, left_offset + width / 2, "Borders On");
            if (config.is_real_border()) attroff(A_BOLD);

            if (!config.is_real_border()) attron(A_BOLD);
            mvaddstr(base + 6, left_offset + width / 2, "Borders Off");
            if (!config.is_real_border()) attroff(A_BOLD);

            for (i = 1; i <= 9; ++i) {
                if (i == config.get_level()) attron(A_BOLD);
                mvaddch(base + 8, left_offset + width / 2 + (i - 1) * 2,
                        i + '0');
                if (i == config.get_level()) attroff(A_BOLD);
            }
            break;
        default:
            break;
    }
    refresh();
}

static Game game_;

void redraw_snack(int signum) {
    // int has_veer = 0;
    struct timeval tv1;
    struct timeval tv2;
//    Point prev_pos{0, 0};

    gettimeofday(&tv1, nullptr);
    /* need veer */
    if (!game_.queue_dir.empty()) {
        int new_direction = game_.queue_dir.front();
        game_.queue_dir.pop();
        game_.the_snake.change_direction(new_direction);
    }
    game_.the_snake.update();
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
    mvaddch(game_.the_snake.head().y, game_.the_snake.head().x, 'x');
    attroff(A_BOLD);
    game_.game_over();
}

void Game::on_game(const Config& config) {
    int set_ticker(int n_msecs);
    int ch;
    int pre_ch;
    int delay;
    gover = 1;
    clear();
    rect.draw(config.is_real_border());

    the_snake.draw();

    draw_fruit();

    signal(SIGALRM, redraw_snack);
    delay = 20 * (10 - config.get_level());
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
                    on_game(config);    /* just recursion */
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
                destroy();
                exit(0);
#endif
            default:
                break;
        }
    }
}

Game::Game(): rect(0, 0, WIN_COLS, WIN_LINES) {
}

void Game::game_over() {
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

void Game::draw_fruit() {
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
int Game::is_hit_body(int flag) {
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
        if (true) { // todo: setting.border == BORDER_OFF
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
