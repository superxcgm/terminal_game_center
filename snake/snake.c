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
#include <signal.h>
#include <time.h>
#include "snake.h"
#include "res.h"
#include "xc_queue.h"

struct snake_setting setting;
struct snake the_snake;
int score;
struct xc_queue queue_dir;
struct xc_point fruit;

int main(void)
{
	init();
	draw_menu();
	before_destory();
	return 0;
}
void init()
{
	initscr();
	if(COLS < WIN_COLS || LINES < WIN_LINES){
		endwin();
		fprintf(stderr, "Your console screen is smaller than %dx%d\n",
			WIN_COLS, WIN_LINES);
		fprintf(stderr, "Please resize your window and try again\n");
		exit(1);
	}
	srand(time(0));
	cbreak(); /* donot buffer input */
	noecho();
	curs_set(0); /* donot display cursor */
	keypad(stdscr, TRUE); /* enable arrow key */
	/* ignore INT and QUIT signals */
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
void before_destory()
{
	curs_set(1); /* display cursor */
	endwin();
}
void draw_menu()
{
	struct XC_Res *res_snake;
	int i;
	int ch;
	int control_menu_base;
	res_snake = load_res("snake.res");
	for(i = 0; i < res_snake->cnt; ++i)
		mvaddstr(i + 3, 3, res_snake->data[i]);
	addstr("   V"VERSION); /* strcat */
	draw_border('*');
	control_menu_base =   3 + res_snake->cnt + 3;
	setting.level = 1;
	setting.border = BORDER_ON;
	draw_control_menu(0, control_menu_base); /* 0=>base, 1=>control part */
	draw_control_menu(1, control_menu_base);
	refresh();
	while(1){
		ch = getch();
		switch(ch){
			case KEY_LEFT:
				if(setting.level > 1){
					--setting.level;
					draw_control_menu(1, control_menu_base);
				}
				break;
			case KEY_RIGHT:
				if(setting.level < 9){
					++setting.level;
					draw_control_menu(1, control_menu_base);
				}
				break;
			case KEY_UP:
				if(setting.border == BORDER_OFF){
					setting.border = BORDER_ON;
					draw_control_menu(1, control_menu_base);
				}
				break;
			case KEY_DOWN:
				if(setting.border == BORDER_ON){
					setting.border = BORDER_OFF;
					draw_control_menu(1, control_menu_base);
				}
				break;
			case 'q': /* quit */
			case 'Q':
				before_destory();
				exit(0);
				break;
			case ' ':
			case '\n':
				on_game();
				break;
		}
	}
}
void on_game()
{
	int set_ticker(int n_msecs);
	int ch;
	int pre_ch;
	score = 0;
	if(xc_queue_init(&queue_dir) == 0){
		before_destory();
		printf("Can not initialize queue_dir.\n");
		exit(1);
	}
	clear(); /* clear screen */
	draw_border(setting.border == BORDER_ON ? '#' : '.');
	/* init snake */
	the_snake.head = (struct node_front*)malloc(sizeof(struct node_front));
	the_snake.head->pos.x = INIT_X;
	the_snake.head->pos.y = INIT_Y;
	the_snake.head->prev = NULL;
	the_snake.tail = (struct node_front*)malloc(sizeof(struct node_front));
	the_snake.tail->pos.y = the_snake.head->pos.y;
	the_snake.tail->pos.x = the_snake.head->pos.x - 3;
	the_snake.tail->prev = the_snake.head;
	the_snake.dir = DIR_RIGHT;

	/* draw snake */
	draw_body_line(&the_snake.head->pos, &the_snake.tail->pos);
	mvaddch(the_snake.head->pos.y, the_snake.head->pos.x, SYMBOL_SNAKE_HEAD);

	draw_fruit();

	signal(SIGALRM, redraw_snack);
	set_ticker(100);	/* speed of snake would not change during play */
	pre_ch = ' ';
	while(1){
		ch = getch();
		if(ch == pre_ch)	/* ignore duplication key press */
			continue;
		switch(ch){
			case 'w':
			case 'W':
			case KEY_UP:
				/* use queue to buffer direction change request, if change
				the_snake.direction directly, later request will cover the
				previous request. */
				if(pre_ch == KEY_DOWN) continue; /* forbid reverse */
				xc_queue_insert(&queue_dir, DIR_UP);
				pre_ch = KEY_UP;
				break;
			case 's':
			case 'S':
			case KEY_DOWN:
				if(pre_ch == KEY_UP) continue; /* forbid reverse */
				xc_queue_insert(&queue_dir, DIR_DOWN);
				pre_ch = KEY_DOWN;
				break;
			case 'a':
			case 'A':
			case KEY_LEFT:
				if(pre_ch == KEY_RIGHT) continue; /* forbid reverse */
				xc_queue_insert(&queue_dir, DIR_LEFT);
				pre_ch = KEY_LEFT;
				break;
			case 'd':
			case 'D':
			case KEY_RIGHT:
				if(pre_ch == KEY_LEFT) continue; /* forbid reverse */
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
void draw_body_line(const struct xc_point *p1, const struct xc_point *p2)
{
	int dir;
	int i;
	const struct xc_point *tmp_point;
	if(p1->x == p2->x){	/* vertical */
		/* make sure that p1->y < p2->y*/
		if(p1->y > p2->y){
			tmp_point = p1;
			p1 = p2;
			p2 = tmp_point;
		}
		for(i = p1->y; i <= p2->y; ++i){
			move(i, p1->x);
			addch(SYMBOL_SNAKE_BODY);	
		}
		
	}else if(p1->y == p2->y){	/* horizontal */
		if(p1->x > p2->x){
			tmp_point = p1;
			p1 = p2;
			p2 = tmp_point;
		}
		move(p1->y, p1->x);
		for(i = p1->x; i <= p2->x; ++i) addch(SYMBOL_SNAKE_BODY);
	}
}
void draw_fruit()
{
	do{
		fruit.x = rand() % (WIN_COLS - 1) + 1;
		fruit.y = rand() % (WIN_LINES - 1) + 1;	
	}while(!is_fuit_legal());
	mvaddch(fruit.y, fruit.x, SYMBOL_FRUIT);
	refresh();
}
int is_fuit_legal()
{
	struct node_front *p;
	struct node_front *prev;
	p = the_snake.tail;
	prev = p->prev;
	while(prev){
		if(p->pos.x == prev->pos.x){
			if((fruit.y >= p->pos.y && fruit.y <= prev->pos.y) ||
				(fruit.y >= prev->pos.y && fruit.y <= p->pos.y))
				return 0;
		}else if(p->pos.y == prev->pos.y){
			if((fruit.x >= p->pos.x && fruit.x <= prev->pos.x) ||
				(fruit.x >= prev->pos.x && fruit.x <= p->pos.x))
				return 0;
		}
		p = prev;
		prev = p->prev;
	}
	return 1;
}
void redraw_snack(int signum)
{
	int new_dir;
	struct node_front *prev;

	/* need veer */
	if(xc_queue_get(&queue_dir, &new_dir) && 
		the_snake.dir != new_dir &&
		!((the_snake.dir == DIR_LEFT && new_dir == DIR_RIGHT) ||
			(the_snake.dir == DIR_RIGHT && new_dir == DIR_LEFT) ||
			(the_snake.dir == DIR_UP && new_dir == DIR_DOWN) ||
			(the_snake.dir == DIR_DOWN && new_dir == DIR_UP))){
		struct xc_point pos;

		pos = the_snake.head->pos;
		the_snake.dir = new_dir;
		the_snake.head = add_to_head(the_snake.head, &pos);
	}
	/* head node step forward */
	mvaddch(the_snake.head->pos.y, the_snake.head->pos.x, SYMBOL_SNAKE_BODY);
	switch(the_snake.dir){
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

	/* last node step forward */
	mvaddch(the_snake.tail->pos.y, the_snake.tail->pos.x, SYMBOL_BLANK);
	prev = the_snake.tail->prev;

	if(prev->pos.y == the_snake.tail->pos.y){
		if(abs(the_snake.tail->pos.x - prev->pos.x) == 1){
			free(the_snake.tail);
			the_snake.tail = prev;
		}else
			the_snake.tail->pos.x += prev->pos.x > the_snake.tail->pos.x ?
				1 : -1;

	}else if(prev->pos.x == the_snake.tail->pos.x){
		if(abs(the_snake.tail->pos.y - prev->pos.y) == 1){
			free(the_snake.tail);
			the_snake.tail = prev;
		}else
			the_snake.tail->pos.y += prev->pos.y > the_snake.tail->pos.y ?
				1 : -1;
	}
	

	refresh();
}
void draw_control_menu(int flag, int base)
{
	const int left_offset = 7;
	const int height = 10;
	const int width  = 53;
	struct XC_Res *res_control_menu;
	int i;
	/* 0=>base, 1=>control part */
	switch(flag){
		case 0:
			res_control_menu = load_res("control_menu.res");
			for(i = 0; i < res_control_menu->cnt; ++i)
				mvaddstr(base + i, left_offset, res_control_menu->data[i]);
			break;
		case 1:
			if(setting.border == BORDER_ON) attron(A_BOLD);	
			mvaddstr(base + 5, left_offset + width / 2, "Borders On");
			if(setting.border == BORDER_ON) attroff(A_BOLD);

			if(setting.border == BORDER_OFF) attron(A_BOLD);
			mvaddstr(base + 6, left_offset + width / 2, "Borders Off");
			if(setting.border == BORDER_OFF) attroff(A_BOLD);

			for(i = 1; i <= 9; ++i){
				if(i == setting.level) attron(A_BOLD);
				mvaddch(base + 8, left_offset + width / 2 + (i - 1) * 2,
					i + '0');
				if(i == setting.level) attroff(A_BOLD);
			}
			break;
	}
	refresh();
}
void draw_border(char ch)
{
	move(0, 0);
	hline(ch, WIN_COLS);	/* top */
	move(WIN_LINES - 1, 0);
	hline(ch, WIN_COLS);	/* bottom */
	move(0, 0);
	vline(ch, WIN_LINES);	/* left */
	move(0, WIN_COLS - 1);
	vline(ch, WIN_LINES);	/* right */
}