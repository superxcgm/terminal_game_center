#include "./menu.h" /* NOLINT */

#include <ncurses.h>

#define VERSION "0.5"

Menu::Menu(const Rect &r)
    : res_snake_("res/snake.res"),
      res_control_menu_("res/control_menu.res"),
      rect_(r) {}

void Menu::DrawBorder(char ch) {
  // todo: use windows to refactor
  move(0, 0);
  hline(ch, rect_.get_width()); /* top */

  move(rect_.bottom(), 0);
  hline(ch, rect_.get_width()); /* bottom */

  move(0, 0);
  vline(ch, rect_.get_height()); /* left */

  move(0, rect_.right());
  vline(ch, rect_.get_height()); /* right */
}

void Menu::DrawControlMenu(int flag, int base, const Config &config) {
  const int left_offset = 7;
  const int width = 53;
  int i;
  /* 0=>base, 1=>control part */
  switch (flag) {
    case 0:

      for (i = 0; i < res_control_menu_.line_count(); ++i)
        mvaddstr(base + i, left_offset, res_control_menu_.get_line(i).c_str());
      break;
    case 1:
      if (config.is_real_wall()) {
        attron(A_BOLD);
      }
      mvaddstr(base + 5, left_offset + width / 2, "Borders On");
      if (config.is_real_wall()) {
        attroff(A_BOLD);
      }

      if (!config.is_real_wall()) {
        attron(A_BOLD);
      }
      mvaddstr(base + 6, left_offset + width / 2, "Borders Off");
      if (!config.is_real_wall()) {
        attroff(A_BOLD);
      }

      for (i = 1; i <= 9; ++i) {
        if (i == config.get_level()) {
          attron(A_BOLD);
        }
        mvaddch(base + 8, left_offset + width / 2 + (i - 1) * 2, i + '0');
        if (i == config.get_level()) {
          attroff(A_BOLD);
        }
      }
      break;
    default:break;
  }
  refresh();
}

Config Menu::DrawMain() {
  int control_menu_base;

  clear();

  res_snake_.Draw(3, 3);

  addstr("   V" VERSION); /* strcat */
  DrawBorder('*');
  control_menu_base = 3 + res_snake_.line_count() + 3;
  Config config;
  DrawControlMenu(0, control_menu_base,
                  config); /* 0=>base, 1=>control part */
  DrawControlMenu(1, control_menu_base, config);
  refresh();
  while (true) {
    int ch = getch();
    switch (ch) {
      case KEY_LEFT:config.DecreaseLevel();
        DrawControlMenu(1, control_menu_base, config);
        break;
      case KEY_RIGHT:config.IncreaseLevel();
        DrawControlMenu(1, control_menu_base, config);
        break;
      case KEY_UP:config.set_real_border(true);
        DrawControlMenu(1, control_menu_base, config);
        break;
      case KEY_DOWN:config.set_real_border(false);
        DrawControlMenu(1, control_menu_base, config);
        break;
      case 'q': /* quit */
      case 'Q':curs_set(1); /* display cursor */
        endwin();
        exit(0);
      case ' ':
      case '\n':return config; /* reenter menu_ */
      default:break;
    }
  }
}
