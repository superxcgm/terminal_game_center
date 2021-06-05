/* bounce2d 1.0
 * become a character(default is 'o') around the screen
 * defined by some parameters
 *
 * user input: `a` move padding to right, `d` move paddle to right
 * 			`Q`/'q' quit
 *
 * blocks on read, but timer tick sends SIGALRM caught by ball_move
 * build: make
 *
 * @author: superxcgm
 * e-mail: superxcgm@gmail.com
 */

#include <curses.h>

#include <csignal>
#include <cstdlib>
#include <cstring>

#include "bounce.h"

struct ppball the_ball;
struct paddle pad;
int score;

/* the main loop */
void set_up();

void wrap_up();

int set_ticker(int n_msecs);

void draw_info();

int main(int argc, char **argv) {
  set_up();

  while (true) {
    int ch = getchar();
    if (ch == 'q' || ch == 'Q') {
      break;
    }
    switch (ch) {
      case 'a':
        pad.x_pos -= PA_STEP;
        if (pad.x_pos < LEFT_EDGE) pad.x_pos = LEFT_EDGE;
        pad.draw();
        break;
      case 'd':
        pad.x_pos += PA_STEP;
        if (pad.x_pos + pad.width >= RIGHT_EDGE)
          pad.x_pos = RIGHT_EDGE - pad.width + 1;
        pad.draw();
        break;
      default:
        break;
    }
  }
  wrap_up();
}

/* init structure and other stuff */
void set_up() {
  void ball_move(int);

  the_ball.init();

  /* init paddle */
  pad.init();
  score = 0;

  initscr();
  keypad(stdscr, true);
  noecho();
  curs_set(0);
  crmode();

  mvaddstr(BOT_ROW + 3, 3,
           "press 'a' or 'd' to move pad, 'q' or 'Q' to quit game.");

  border(DEFAULT_CHAR, DEFAULT_CHAR, DEFAULT_CHAR, BLANK, DEFAULT_CHAR,
         DEFAULT_CHAR, BLANK, BLANK);
  move(TOP_ROW, RIGHT_EDGE + 1);
  vline(0, RIGHT_EDGE);

  signal(SIGINT, SIG_IGN);

  the_ball.draw();
  pad.draw();

  draw_info();
  refresh();

  signal(SIGALRM, ball_move);
  set_ticker(1000 / TICKS_PER_SEC); /* send milliseconds per tick */
}

void wrap_up() {
  set_ticker(0);
  endwin(); /* put back to normal */
}

void ball_move(int signum) {
  int bounce_or_lost(struct ppball * bp);
  int y_cur, x_cur, moved;

  signal(SIGALRM, SIG_IGN);
  y_cur = the_ball.y_pos;
  x_cur = the_ball.x_pos;
  moved = 0;

  if (the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1) {
    the_ball.y_pos += the_ball.y_dir;
    the_ball.y_ttg = the_ball.y_ttm;
    moved = 1;
  }

  if (the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1) {
    the_ball.x_pos += the_ball.x_dir;
    the_ball.x_ttg = the_ball.x_ttm;
    moved = 1;
  }

  if (moved) {
    mvaddch(y_cur, x_cur, BLANK);
    mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
    if (bounce_or_lost(&the_ball) == -1) {
      const char *str = "Game Over!";
      mvaddstr((TOP_ROW + BOT_ROW) / 2,
               (LEFT_EDGE + RIGHT_EDGE - strlen(str)) / 2, str);
      /* move ball the next row means it out of boundry */
      mvaddch(the_ball.y_pos, the_ball.x_pos, BLANK);
      mvaddch(the_ball.y_pos + 1, the_ball.x_pos, the_ball.symbol);
      set_ticker(0); /* turn off ticker */
    }
    move(LINES - 1, COLS - 1);
    refresh();
  }
  signal(SIGALRM, ball_move); /* for unreliable systems */
}

/* return 0 => normal, 1 => bounce, -1 game over */
int bounce_or_lost(struct ppball *bp) {
  int return_val = 0;

  if (bp->y_pos == TOP_ROW) {
    bp->y_dir = 1;
    return_val = 1;
  } else if (bp->y_pos == BOT_ROW) {
    if (bp->x_pos >= pad.x_pos && bp->x_pos <= pad.x_pos + pad.width - 1) {
      /* hit paddle and bounce */
      if (bp->y_dir == 1) {
        score += 50; /* receive a ball */
        /* speed up */
        if (score % 150 == 0) {
          the_ball.x_ttm -= 2;
          the_ball.y_ttm -= 2;
        }
        draw_info();
      }
      bp->y_dir = -1;
      return_val = 1;
    } else /* game over */
      return -1;
  }

  if (bp->x_pos == LEFT_EDGE) {
    bp->x_dir = 1;
    return_val = 1;
  } else if (bp->x_pos == RIGHT_EDGE) {
    bp->x_dir = -1;
    return_val = 1;
  }
  return return_val;
}

void draw_info() {
  static char str[50];
  sprintf(str, "Score: %-4d", score);
  mvaddstr(INFO_Y, INFO_X, str);
}