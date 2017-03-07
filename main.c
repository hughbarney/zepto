/* main.c, Atto Emacs, Public Domain, Hugh Barney, 2016, Derived from: Anthony's Editor January 93 */

#include "header.h"

int done;
char_t *input;
int msgflag;
char msgline[TEMPBUF];
char temp[TEMPBUF];

keymap_t *key_return;
keymap_t *key_map;
buffer_t *curbp;
window_t *curwp;

int main(int argc, char **argv)
{
	if (argc != 2) fatal("usage: zepto filename\n");

	initscr();
	raw();
	
	curbp = new_buffer();
	(void)insert_file(argv[1], FALSE);
	/* Save filename irregardless of load() success. */
	strncpy(curbp->b_fname, argv[1], NAME_MAX);
	curbp->b_fname[NAME_MAX] = '\0'; /* force truncation */
	curwp = new_window(curbp);

	if (!growgap(curbp, CHUNK))
		fatal("Failed to allocate required memory.\n");

	top();
	key_map = keymap;

	while (!done) {
		display(curwp);
		input = get_key(key_map, &key_return);

		if (key_return != NULL) {
			(key_return->func)();
		} else {
			/*
			 * if first char of input is a control char then
			 * key is not bound, except TAB and NEWLINE
			 */
			if (*input > 31 || *input == 0x0A || *input == 0x09)
				insert();
                        else {
				fflush(stdin);
				msg("not bound");
			}
		}
	}

	move(MSGLINE, 0);
	clrtoeol();
	refresh();
	noraw();
	return 0;
}

void fatal(char *msg)
{
	fprintf(stderr, "\nzepto: %s\n", msg);
	exit(1);
}

void msg(char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	(void)vsprintf(msgline, msg, args);
	va_end(args);
	msgflag = TRUE;
}

buffer_t* new_buffer()
{
	buffer_t *bp = (buffer_t *)malloc(sizeof(buffer_t));
	assert(bp != NULL);

	bp->b_point = 0;
	bp->b_page = 0;
	bp->b_epage = 0;
	bp->b_flags = 0;
	bp->b_buf = NULL;
	bp->b_ebuf = NULL;
	bp->b_gap = NULL;
	bp->b_egap = NULL;
	bp->b_fname[0] = '\0';
	return bp;
}

window_t* new_window(buffer_t *bp)
{
	window_t *wp = (window_t *)malloc(sizeof(window_t));
	
	assert(wp != NULL);
	wp->w_bufp = bp;
	wp->w_top = 0;	
	wp->w_rows = LINES - 2;
	return wp;
}
