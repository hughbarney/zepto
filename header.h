/* header.h, Zepto Emacs, Public Domain, Hugh Barney, 2017, Derived from: Anthony's Editor January 93 */

#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include "raw.h"

#define B_MODIFIED	0x01		/* modified buffer */
#define MSGLINE         (LINES-1)
#define CHUNK           8096L
#define K_BUFFER_LENGTH 256
#define TEMPBUF         512
#define MIN_GAP_EXPAND  512
#define NOMARK          -1

typedef unsigned char char_t;
typedef long point_t;

typedef struct keymap_t {
	char *key_desc;                 /* name of bound function */
	char *key_bytes;		/* the string of bytes when this key is pressed */
	void (*func)(void);
} keymap_t;

typedef struct buffer_t
{
	point_t b_mark;	     	  /* the mark */
	point_t b_point;          /* the point */
	point_t b_page;           /* start of page */
	point_t b_epage;          /* end of page */
	char_t *b_buf;            /* start of buffer */
	char_t *b_ebuf;           /* end of buffer */
	char_t *b_gap;            /* start of gap */
	char_t *b_egap;           /* end of gap */
	char w_top;	          /* Origin 0 top row of window */
	char w_rows;              /* no. of rows of text in window */
	int b_row;                /* cursor row */
	int b_col;                /* cursor col */
	char b_fname[NAME_MAX + 1]; /* filename */
	char b_flags;             /* buffer flags */
} buffer_t;

extern buffer_t *curbp;			/* current buffer */

/*
 * Some compilers define size_t as a unsigned 16 bit number while
 * point_t and off_t might be defined as a signed 32 bit number.  
 * malloc(), realloc(), fread(), and fwrite() take size_t parameters,
 * which means there will be some size limits because size_t is too
 * small of a type.
 */
#define MAX_SIZE_T      ((unsigned long) (size_t) ~0)

extern int done;                /* Quit flag. */
extern int msgflag;             /* True if msgline should be displayed. */
extern char_t *input;
extern char msgline[];          /* Message line input/output buffer. */
extern char temp[];             /* Temporary buffer. */
extern keymap_t *key_map;       /* Command key mappings. */
extern keymap_t keymap[];
extern keymap_t *key_return;    /* Command key return */

extern void fatal(char *);
extern void msg(char *, ...);
extern void display(void);
extern void dispmsg(void);
extern void modeline(buffer_t *);
extern point_t lnstart(buffer_t *, point_t);
extern point_t lncolumn(buffer_t *, point_t, int);
extern point_t segstart(buffer_t *, point_t, point_t);
extern point_t segnext(buffer_t *, point_t, point_t);
extern point_t upup(buffer_t *, point_t);
extern point_t dndn(buffer_t *, point_t);
extern char_t *get_key(keymap_t *, keymap_t **);
extern int growgap(buffer_t *, point_t);
extern point_t movegap(buffer_t *, point_t);
extern point_t pos(buffer_t *, char_t *);
extern char_t *ptr(buffer_t *, point_t);
extern void save(void);
extern int insert_file(char *, int);
extern void backsp(void);
extern void bottom(void);
extern void delete(void);
extern void down(void);
extern void insert(void);
extern void left(void);
extern void lnbegin(void);
extern void lnend(void);
extern void pgdown(void);
extern void pgup(void);
extern void quit(void);
extern void right(void);
extern void top(void);
extern void up(void);
extern void killtoeol(void);
extern buffer_t* new_buffer(void);
extern void search(void);
point_t search_forward(buffer_t *, point_t, char *);
extern void display_search_result(point_t, char *, char *);
extern void cut(void);
extern void copy(void);
extern void copy_cut(int);
extern void paste(void);
extern void set_mark(void);
