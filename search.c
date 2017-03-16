/* search.c, Zepto Emacs, Public Domain, Hugh Barney, 2016 */

#include "header.h"

#define STRBUF_M   64
char searchtext[STRBUF_M];

void search()
{
	int cpos = 0;	
	int c;
	point_t o_point = curbp->b_point;
	point_t found;

	searchtext[0] = '\0';
	msg("Search: %s", searchtext);
	dispmsg();
	cpos = strlen(searchtext);

	for (;;) {
		refresh();
		c = getch();
		/* ignore control keys other than C-g, backspace, CR,  C-s, C-R, ESC */
		if (c < 32 && c != 07 && c != 0x08 && c != 0x13 && c != 0x12 && c != 0x1b)
			continue;

		switch(c) {
		case 0x1b: /* esc */
			searchtext[cpos] = '\0';
			flushinp(); /* discard any escape sequence without writing in buffer */
			return;
		case 0x07: /* ctrl-g */
			curbp->b_point = o_point;
			return;
		case 0x13: /* ctrl-s, do the search */
			found = search_forward(curbp, curbp->b_point, searchtext);
			if (found != -1 ) {
				curbp->b_point = found;
				msg("Search: %s", searchtext);
				display();
			} else {
				msg("Failing Search: %s", searchtext);
				dispmsg();
				curbp->b_point = 0;
			}
			break;
		case 0x7f: /* del, erase */
		case 0x08: /* backspace */
			if (cpos == 0)
				continue;
			searchtext[--cpos] = '\0';
			msg("Search: %s", searchtext);
			dispmsg();
			break;
		default:	
			if (cpos < STRBUF_M - 1) {
				searchtext[cpos++] = c;
				searchtext[cpos] = '\0';
				msg("Search: %s", searchtext);
				dispmsg();
			}
			break;
		}
	}
}

point_t search_forward(buffer_t *bp, point_t start_p, char *stext)
{
	point_t end_p = pos(bp, bp->b_ebuf);
	point_t p,pp;
	char* s;

	if (0 == strlen(stext))
		return start_p;

	for (p=start_p; p < end_p; p++) {
		for (s=stext, pp=p; *s == *(ptr(bp, pp)) && *s !='\0' && pp < end_p; s++, pp++)
			;

		if (*s == '\0')
			return pp;
	}

	return -1;
}
