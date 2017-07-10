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
#include "snake.h"
#include "res.h"

struct snake_setting setting;

int main(void)
{
	void init();
	void before_destory();
	void draw_menu();

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
		fprintf(stderr, "Your console screen is smaller than %dx%d\n", WIN_COLS, WIN_LINES);
		fprintf(stderr, "Please resize your window and try again\n");
		exit(1);
	}
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
	void draw_border(char ch);
	void draw_control_menu(int flag, int base);
	int i;
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
		int ch = getch();
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
				mvaddstr(0, 0, "enter the game.");
				break;
		}
	}
}
void draw_control_menu(int flag, int base)
{
	const int left_offset = 7;
	const int height = 10;
	const int width  = 53;
	struct XC_Res *res_control_menu;
	int i;

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
				mvaddch(base + 8, left_offset + width / 2 + (i - 1) * 2, i + '0');
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