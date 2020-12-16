#include <ncurses.h>
int main()
{
    WINDOW *win;
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    refresh();
    win = newwin(10, 10, 1, 1);
    wbkgd(win, COLOR_PAIR(1));
    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, 1, 1, "A new window");
    wborder(win, '*','*','*','*','*','*','*','*');
    wrefresh(win);
    getch();
    delwin(win);
    endwin();

}
