/* raw.h, Zepto Emacs, Public Domain, Hugh Barney, 2017, Derived from: Anthony's Editor January 93  *
 * emulate ncurses functions using raw ttyio and VT100 escape sequences                             */

extern int LINES;
extern int COLS;

void initscr(void);
void addch(char);
void addstr(char *);
void refresh(void);
void clear(void);
void clrtoeol(void);
void standout(void);
void standend(void);
void move(int, int);
void mvaddstr(int, int, char *);
char getch();
void raw(void);
void noraw(void);
char *unctrl(char );
void flushinp(void);

#define FALSE 0
#define TRUE !FALSE

#define CLRSCR           "\033[2J"
#define RESET_TTY        "\033c"
#define HIDE_CURSOR      "\033[?25l"
#define SHOW_CURSOR      "\033[?25h"
#define CLREOL           "\033[K"
#define GOTOXY           "\033[%d;%dH"
#define TEXT_COLOR       "\033[0m"
#define STATUS_COLOR     "\033[7m"
