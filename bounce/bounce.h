/* bounce.h */

/* some settings for the game */

#define BLANK ' '
#define BA_DFL_SYMBOL 'O'
#define H_SYMBOL '-'
#define V_SYMBOL '|'
#define TOP_ROW 5
#define BOT_ROW 20
#define LEFT_EDGE 10
#define RIGHT_EDGE 70
#define INFO_X (RIGHT_EDGE + 5)
#define INFO_Y (TOP_ROW + 2)
#define X_INIT 10	/* starting col */
#define Y_INIT 10	/* starting row */
#define TICKS_PER_SEC 200	/* affects speed */
#define PA_DFL_WIDTH 8
#define PA_DFL_SYMBOL '='
#define PA_STEP 3

#define X_TIM 20
#define Y_TIM 35

/* the ping pong ball */
struct ppball{
	int y_pos;
	int x_pos;

	int y_ttm;
	int x_ttm;

	int y_ttg;
	int x_ttg;

	int y_dir;
	int x_dir;

	char symbol;
};
struct paddle{
	int width;
	int x_pos;
	int y_pos;
	char symbol;
};