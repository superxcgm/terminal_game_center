#include <ncurses.h>

void init()
{
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void draw_board()
{
    int max_row, max_col;
    max_row = max_col = 16;
    // getmaxyx(stdscr, max_row, max_col);

    // print first line
    addch(ACS_ULCORNER);
    for (int i = 1; i < max_col - 1; i++)
    {
        addch(ACS_TTEE);
    }
    addch(ACS_URCORNER);

    // print middle lines
    for (int i = 1; i < max_row - 1; i++)
    {
        move(i, 0);
        addch(ACS_LTEE);
        for (int j = 1; j < max_col - 1; j++)
        {
            addch(ACS_PLUS);
        }
        addch(ACS_RTEE);
    }

    // last line
    move(max_row - 1, 0);
    addch(ACS_LLCORNER);
    for (int i = 1; i < max_col - 1; i++)
    {
        addch(ACS_BTEE);
    }
    addch(ACS_LRCORNER);

    mvaddch(8, 8, 'X');
    mvaddch(8, 7, 'O');
    refresh();
}

int main()
{
    init();

    draw_board();

    getch();
    endwin();

    return 0;
}