/* raw.c, Zepto Emacs, Public Domain, Hugh Barney, 2017, Derived from: Anthony's Editor January 93 *
 * emulate ncurses functions using raw ttyio and VT100 escape sequences                            */

#include <sys/ioctl.h>
#include <termios.h>
#include "header.h"

struct termios ostate, nstate;

int LINES;
int COLS;

void clear() { addstr(CLRSCR); } 
void clrtoeol() { addstr(CLREOL); }
void standout() { addstr(STATUS_COLOR); }
void standend() { addstr(TEXT_COLOR); }
char getch() { return fgetc(stdin); }
void flushinp() { fflush(stdin); }

/* Simple buffer structure to append all output to write in one go by calling refresh *
 * This avoids flickering.                                                            */
struct screen_buf {
	char *buf;
	int len;
};

struct screen_buf sb  = {NULL, 0};

void sb_append(struct screen_buf *b, const char *s, int len) {
	char *new = realloc(b->buf, b->len+len);
	if (new == NULL) return;
	memcpy(new + b->len,s,len);
	b->buf = new;
	b->len += len;
}

void sb_free(struct screen_buf *b) {
	if (b-> buf != NULL) free(b->buf);
	b->buf = NULL;
	b->len = 0;
}

void addch(char c)
{
	char ch[2];
	ch[0] = c;
	sb_append(&sb, ch, 1);
}

void addstr(char *str) { sb_append(&sb, str, strlen(str)); }

void refresh()
{
	if (sb.len > 0) fwrite(sb.buf, 1, sb.len, stdout);
	sb_free(&sb);
}

void initscr()
{
	struct winsize ws;
	ioctl(0, TIOCGWINSZ, &ws);
	COLS = ws.ws_col;
	LINES = ws.ws_row - 1;
	addstr(RESET_TTY);
	addstr(TEXT_COLOR);
	clear();
	refresh();
}

void move(int line, int col) 
{
	char buf[32];
	sprintf(buf, GOTOXY, line + 1, col + 1);
	addstr(buf);
}

char unctrl_str[3];

// FIXME
char *unctrl(char c)
{
	unctrl_str[0] = '^';
	unctrl_str[1] = '$';
	unctrl_str[2] = '\0';
	return unctrl_str;
}

void mvaddstr(int r, int c, char *s)
{
	move(r,c);
	addstr(s);
}

void raw()
{
	/* save terminal flags */
	if ((tcgetattr(0, &ostate) < 0) || (tcgetattr(0, &nstate) < 0))
		fatal("Can't read terminal capabilites\n");

	cfmakeraw(&nstate);		/* set raw mode */

	nstate.c_cc[VMIN] = 1;
	nstate.c_cc[VTIME] = 0;	/* block indefinitely for a single char */

	if (tcsetattr(0, TCSADRAIN, &nstate) < 0)
		fatal("Can't set terminal mode\n");
}

void noraw()
{
	if (tcsetattr(0, TCSADRAIN, &ostate) < 0)
		fatal("Can't restore terminal flags");
}
