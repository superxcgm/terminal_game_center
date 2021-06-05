#ifndef BOUNCE_BOUNCE_H_
#define BOUNCE_BOUNCE_H_

#include <utility>

/* some settings for the game */

#define BLANK ' '
#define DEFAULT_CHAR 0
#define BA_DFL_SYMBOL 'O'
#define TOP_ROW 1
#define BOT_ROW 20
#define LEFT_EDGE 1
#define RIGHT_EDGE 70
#define INFO_X (RIGHT_EDGE + 5)
#define INFO_Y (TOP_ROW + 2)
#define X_INIT 2          /* starting col */
#define Y_INIT 2          /* starting row */
#define TICKS_PER_SEC 200 /* affects speed */
#define PA_DFL_WIDTH 8
#define PA_DFL_SYMBOL '='
#define PA_STEP 3

#define X_TIM 20
#define Y_TIM 35

/* the ping pong ball */
struct ppball {
  int y_pos;
  int x_pos;

  int y_ttm;
  int x_ttm;

  int y_ttg;
  int x_ttg;

  int y_dir;
  int x_dir;

  char symbol;

  void init() {
    y_pos = Y_INIT;
    x_pos = X_INIT;
    y_ttg = y_ttm = Y_TIM;
    x_ttg = x_ttm = X_TIM;
    y_dir = 1;
    x_dir = 1;
    symbol = BA_DFL_SYMBOL;
  }

  void draw() { mvaddch(y_pos, x_pos, symbol); }
};

struct paddle {
  int width;
  int x_pos;
  int y_pos;
  char symbol;

  void init() {
    width = PA_DFL_WIDTH;
    x_pos = (LEFT_EDGE + RIGHT_EDGE + PA_DFL_WIDTH) / 2;
    y_pos = BOT_ROW + 1;
    symbol = PA_DFL_SYMBOL;
  }

  void draw() {
    int start_pos = x_pos - PA_STEP - 1;
    int end_pos = start_pos + width + 2 * PA_STEP + 2;
    if (start_pos <= LEFT_EDGE) start_pos = LEFT_EDGE;
    if (end_pos >= RIGHT_EDGE) end_pos = RIGHT_EDGE + 1;

    move(y_pos, start_pos);
    hline(BLANK, end_pos - start_pos);
    move(y_pos, x_pos);
    hline(symbol, width);
  }
};

#endif  // BOUNCE_BOUNCE_H_
