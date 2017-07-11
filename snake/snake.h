#ifndef XC_LINKLIST
	#include "linklist.h"
#endif

#define WIN_LINES			24
#define WIN_COLS			80
#define VERSION				"0.5"
#define BORDER_ON			1
#define BORDER_OFF			0
#define INIT_X				40
#define INIT_Y				12
#define INIT_LEN			3
#define DIR_LEFT			1
#define DIR_RIGHT			2
#define DIR_UP				3
#define DIR_DOWN			4
#define SYMBOL_SNAKE_BODY	'o'
#define SYMBOL_SNAKE_HEAD	'@'
#define SYMBOL_FRUIT 		'$'
#define SYMBOL_BLANK		' '
#define SYMBOL_BORDER_ON	'#'
#define SYMBOL_BORDER_OFF	'.'
#define ONDEBUG

struct snake_setting{
	int level;
	int border;
};

struct snake{
	/* use Singly Linked List to storage veer points of snake */
	struct node_front *head;
	struct node_front *tail;
	int dir;
};
void init();
void before_destory();
int draw_menu();
void draw_fruit();
int is_hit_wall();
void print_snake();
int is_hit_body(int flag);
void game_over();
void draw_body_line(const struct xc_point *p1, const struct xc_point *p2);
void draw_border(char ch);
void draw_control_menu(int flag, int base);
void on_game();
void redraw_snack(int signum);
void load_all_res();