/* bounce2d 1.0
 * become a character(default is 'o') around the screen
 * defined by some parameters
 *
 * user input: `a` move padding to right, `d` move paddle to right
 * 			`Q` quit
 *
 * blocks on read, but timer tick sends SIGALRM caught by ball_move
 * build: gcc -lcurses -o bounce2d bounce2d.c set_ticker.c
 *
 * @author: superxc
 * e-mail: superxc@outlook.com
 */

#include <curses.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "bounce.h"

struct ppball the_ball;
struct paddle pad;
int score;

/* the main loop */
void set_up();
void wrap_up();
int set_ticker(int n_msecs);
void draw_paddle();
void draw_info();

int main()
{
	int c;

	set_up();

	while((c = getchar()) != 'Q'){
		switch(c){
			// case 'f':
			// 	the_ball.x_ttm--;
			// 	break;
			// case 'F':
			// 	the_ball.y_ttm--;
			// 	break;
			// case 's':
			// 	the_ball.x_ttm++;
			// 	break;
			// case 'S':
			// 	the_ball.y_ttm++;
			// 	break;
			case 'a':
				// mvaddstr(0, 0, "receive left KEY");
				pad.x_pos -= PA_STEP;
				if(pad.x_pos < LEFT_EDGE)
					pad.x_pos = LEFT_EDGE;
				draw_paddle();
				break;
			case 'd':
				// mvaddstr(0, 0, "receive right KEY");
				pad.x_pos += PA_STEP;
				if(pad.x_pos + pad.width >= RIGHT_EDGE)
					pad.x_pos = RIGHT_EDGE - pad.width + 1;
				draw_paddle();
				break;
		}
	}
	wrap_up();
}
/* init structure and other stuff */
void set_up()
{
	void ball_move(int);

	/* init ball */
	the_ball.y_pos = Y_INIT;
	the_ball.x_pos = X_INIT;
	the_ball.y_ttg = the_ball.y_ttm = Y_TIM;
	the_ball.x_ttg = the_ball.x_ttm = X_TIM;
	the_ball.y_dir = 1;
	the_ball.x_dir = 1;
	the_ball.symbol = BA_DFL_SYMBOL;

	/* init paddle */
	pad.width = PA_DFL_WIDTH;
	pad.x_pos = (LEFT_EDGE + RIGHT_EDGE + PA_DFL_WIDTH) / 2;
	pad.y_pos = BOT_ROW + 1;
	pad.symbol = PA_DFL_SYMBOL;

	score = 0;

	initscr();
	keypad(stdscr, TRUE); /* prepare for receive arrow key, (seems cannot receive array key) */
	noecho(); /* noecho what user type */
	crmode(); /* disable canonical mode(i.e. turn off input buffer) */

	/* draw boundry */
	move(TOP_ROW - 1, LEFT_EDGE - 1);
	hline(H_SYMBOL, RIGHT_EDGE - LEFT_EDGE + 3);
	/* do not have bottom boundry */
	// move(BOT_ROW + 1, LEFT_EDGE - 1);
	// hline(H_SYMBOL, RIGHT_EDGE - LEFT_EDGE + 3);
	move(TOP_ROW, LEFT_EDGE - 1);
	vline(V_SYMBOL, BOT_ROW - TOP_ROW + 2);
	move(TOP_ROW, RIGHT_EDGE + 1);
	vline(V_SYMBOL, BOT_ROW - TOP_ROW + 2);

	signal(SIGINT, SIG_IGN);

	/* draw ball */
	mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
	draw_paddle();

	/* draw info frame */
	draw_info();
	refresh();

	signal(SIGALRM, ball_move);
	set_ticker(1000 / TICKS_PER_SEC); /* send millisecs per tick */
}
void wrap_up()
{
	set_ticker(0);
	endwin();	/* put back to normal */
}
void ball_move(int signum)
{
	int bounce_or_lost(struct ppball *bp);
	int y_cur, x_cur, moved;

	signal(SIGALRM, SIG_IGN);	/* don't get caught noew */
	y_cur = the_ball.y_pos;		/* old spot */
	x_cur = the_ball.x_pos;
	moved = 0;

	if(the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1){
		the_ball.y_pos += the_ball.y_dir;	/* move */
		the_ball.y_ttg = the_ball.y_ttm;	/* reset */
		moved = 1;
	}

	if(the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1){
		the_ball.x_pos += the_ball.x_dir;	/* move */
		the_ball.x_ttg = the_ball.x_ttm;	/* reset */
		moved = 1;
	}

	if(moved){
		mvaddch(y_cur, x_cur, BLANK);
		// mvaddch(y_cur, x_cur, BLANK);
		mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
		// bounce_or_lost(&the_ball);
		if(bounce_or_lost(&the_ball) == -1){
			const char *str = "Game Over!";
			mvaddstr((TOP_ROW + BOT_ROW) / 2,
			 (LEFT_EDGE + RIGHT_EDGE - strlen(str)) / 2,
			 str);
			/* move ball the next row means it out of boundry */
			mvaddch(the_ball.y_pos, the_ball.x_pos, BLANK);
			mvaddch(the_ball.y_pos + 1, the_ball.x_pos, the_ball.symbol);
			set_ticker(0); /* turn off ticker */
		}
		move(LINES - 1, COLS - 1);
		refresh();
	}
	signal(SIGALRM, ball_move);				/* for unreliable systems */
}
/* return 0 => normal, 1 => bounce, -1 game over */
int bounce_or_lost(struct ppball *bp)
{
	int return_val = 0;

	if(bp->y_pos == TOP_ROW){
		bp->y_dir = 1;
		return_val = 1;
	}else if(bp->y_pos == BOT_ROW){
		
		if(bp->x_pos >= pad.x_pos && bp->x_pos <= pad.x_pos + pad.width - 1){
			/* hit paddle and bounce */
			if(bp->y_dir == 1){
				score += 50; /* receive a ball */
				/* speed up */
				if(score % 150 == 0){
					the_ball.x_ttm -= 2;
					the_ball.y_ttm -= 2;	
				}
				draw_info();
			}
			bp->y_dir = -1;
			return_val = 1;
		}else /* game over */
			return -1;
		
	}

	if(bp->x_pos == LEFT_EDGE){
		bp->x_dir = 1;
		return_val = 1;
	}else if(bp->x_pos == RIGHT_EDGE){
		bp->x_dir = -1;
		return_val = 1;
	}
	return return_val;
}
void draw_paddle()
{
	int start_pos = pad.x_pos - PA_STEP - 1;
	int end_pos = start_pos + pad.width + 2 * PA_STEP + 2;
	if(start_pos <= LEFT_EDGE)
		start_pos = LEFT_EDGE;
	if(end_pos >= RIGHT_EDGE)
		end_pos = RIGHT_EDGE + 1;

	move(pad.y_pos, start_pos);
	hline(BLANK, end_pos - start_pos);
	move(pad.y_pos, pad.x_pos);
	hline(pad.symbol, pad.width);	
}
void draw_info()
{
	static char str[50];
	sprintf(str, "Score: %-4d", score);
	mvaddstr(INFO_Y, INFO_X, str);
}