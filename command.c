/* command.c, Atto Emacs, Public Domain, Hugh Barney, 2016, Derived from: Anthony's Editor January 93 */

#include "header.h"

void top()
{
	curbp->b_point = 0;
}

void bottom()
{
	curbp->b_epage = curbp->b_point = pos(curbp, curbp->b_ebuf);
}

void quit_ask()
{
	if (curbp->b_flags & B_MODIFIED) {
		mvaddstr(MSGLINE, 0, "file was modified; really exit (y/n) ?");
		clrtoeol();
		if (!yesno(0))
			return;
	}
	quit();
}

/* flag = default answer, FALSE=n, TRUE=y */
int yesno(int flag)
{
	int ch;

	addstr(flag ? " y\b" : " n\b");
	refresh();
	ch = getch();
	if (ch == '\r' || ch == '\n')
		return (flag);
	return (tolower(ch) == 'y');
}

void quit()
{
	done = 1;
}

void redraw()
{
	clear();
	display(curwp);
}

void left()
{
	if (0 < curbp->b_point)
		--curbp->b_point;
}

void right()
{
	if (curbp->b_point < pos(curbp, curbp->b_ebuf))
		++curbp->b_point;
}

void up()
{
	curbp->b_point = lncolumn(curbp, upup(curbp, curbp->b_point),curbp->b_col);
}

void down()
{
	curbp->b_point = lncolumn(curbp, dndn(curbp, curbp->b_point),curbp->b_col);
}

void lnbegin()
{
	curbp->b_point = segstart(curbp, lnstart(curbp,curbp->b_point), curbp->b_point);
}

void lnend()
{
	curbp->b_point = dndn(curbp, curbp->b_point);
	left();
}

void pgdown()
{
	curbp->b_page = curbp->b_point = upup(curbp, curbp->b_epage);
	while (0 < curbp->b_row--)
		down();
	curbp->b_epage = pos(curbp, curbp->b_ebuf);
}

void pgup()
{
	int i = curwp->w_rows;
	while (0 < --i) {
		curbp->b_page = upup(curbp, curbp->b_page);
		up();
	}
}

void insert()
{
	assert(curbp->b_gap <= curbp->b_egap);
	if (curbp->b_gap == curbp->b_egap && !growgap(curbp, CHUNK))
		return;
	curbp->b_point = movegap(curbp, curbp->b_point);

	*curbp->b_gap++ = *input == '\r' ? '\n' : *input;
	curbp->b_point = pos(curbp, curbp->b_egap);
	curbp->b_flags |= B_MODIFIED;
}

void backsp()
{
	curbp->b_point = movegap(curbp, curbp->b_point);
	if (curbp->b_buf < curbp->b_gap) {
		--curbp->b_gap;
		curbp->b_flags |= B_MODIFIED;
	}
	curbp->b_point = pos(curbp, curbp->b_egap);
}

void delete()
{
	curbp->b_point = movegap(curbp, curbp->b_point);
	if (curbp->b_egap < curbp->b_ebuf) {
		curbp->b_point = pos(curbp, ++curbp->b_egap);
		curbp->b_flags |= B_MODIFIED;
	}
}

void savebuffer()
{
	save(curbp->b_fname);
	refresh();
}

void killtoeol()
{
	/* point = start of empty line or last char in file */
	if (*(ptr(curbp, curbp->b_point)) == 0xa || (curbp->b_point + 1 == ((curbp->b_ebuf - curbp->b_buf) - (curbp->b_egap - curbp->b_gap))) ) {
		delete();
	} else {
		int i = 0;
		while (*(ptr(curbp, curbp->b_point)) != 0xa && i++ < 1000)
			delete();
	}
}
