#include <ncurses.h>

#include "menu.h"

#define VERSION "0.5"

Menu::Menu(const Rect & r) : res_snake("res/Snake.res"),
               res_control_menu("res/control_menu.res"), rect(r){

}

void Menu::draw_border(char ch) {
    // todo: use windows to refactor
    move(0, 0);
    hline(ch, rect.get_width());    /* top */

    move(rect.bottom(), 0);
    hline(ch, rect.get_width());    /* bottom */

    move(0, 0);
    vline(ch, rect.get_height());    /* left */

    move(0, rect.right());
    vline(ch, rect.get_height());    /* right */
}

void Menu::draw_control_menu(int flag, int base, const Config& config) {
    const int left_offset = 7;
    const int width = 53;
    int i;
    /* 0=>base, 1=>control part */
    switch (flag) {
        case 0:

            for (i = 0; i < res_control_menu.line_count(); ++i)
                mvaddstr(base + i, left_offset, res_control_menu.get_line(i).c_str());
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

Config Menu::draw_main() {
    int control_menu_base;

    clear();

    res_snake.draw(3, 3);

    addstr("   V" VERSION); /* strcat */
    draw_border('*');
    control_menu_base = 3 + res_snake.line_count() + 3;
    Config config;
    draw_control_menu(0, control_menu_base, config); /* 0=>base, 1=>control part */
    draw_control_menu(1, control_menu_base, config);
    refresh();
    while (true) {
        int ch = getch();
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
                exit(0);
            case ' ':
            case '\n':
                return config;    /* reenter menu */
            default:
                break;
        }
    }
}
