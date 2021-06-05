#include <ncurses.h>

void init() {
  initscr();
  raw();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
}

void draw_line(int first_ch, int middle_ch, int last_ch, int length) {
  addch(first_ch);
  for (int i = 1; i < length / 2 - 1; i++) {
    addch(ACS_HLINE);
    addch(middle_ch);
  }
  addch(ACS_HLINE);
  addch(last_ch);
}

void draw_board() {
  int max_row, max_col;
  max_row = max_col = 15;
  max_col <<= 1;

  // print first line
  draw_line(ACS_ULCORNER, ACS_TTEE, ACS_URCORNER, max_col);

  // print middle lines
  for (int i = 1; i < max_row - 1; i++) {
    move(i, 0);
    draw_line(ACS_LTEE, ACS_PLUS, ACS_RTEE, max_col);
  }

  // print last line
  move(max_row - 1, 0);
  draw_line(ACS_LLCORNER, ACS_BTEE, ACS_LRCORNER, max_col);

  mvaddch(8, 8, 'X');
  mvaddch(8, 6, 'O');
  mvaddch(7, 6, 'O');
  refresh();
}

int main() {
  init();

  draw_board();

  getch();
  endwin();

  return 0;
}
