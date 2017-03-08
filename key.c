/* key.c, Zepto Emacs, Public Domain, Hugh Barney, 2017, Derived from: Anthony's Editor January 93 */

#include "header.h"

/* desc, keys, func */
keymap_t keymap[] = {
	{"up previous-line         ", "\x1B\x5B\x41", up },
	{"down next-line           ", "\x1B\x5B\x42", down },
	{"left backward-character  ", "\x1B\x5B\x44", left },
	{"right forward-character  ", "\x1B\x5B\x43", right },
	{"C-a beginning-of-line    ", "\x01", lnbegin },
	{"C-e end-of-line          ", "\x05", lnend },
	{"home beginning-of-line   ", "\x1B\x4F\x48", lnbegin }, /* home key */
	{"end end-of-line          ", "\x1B\x4F\x46", lnend }, /* end key */
	{"C-b                      ", "\x02", left },
	{"C-f                      ", "\x06", right },
	{"C-n                      ", "\x0E", down },
	{"C-p                      ", "\x10", up },
	{"C-d forward-delete-char  ", "\x04", delete },
	{"DEL forward-delete-char  ", "\x1B\x5B\x33\x7E", delete }, /* Del key */
	{"backspace delete-left    ", "\x7f", backsp },
	{"C-h backspace            ", "\x08", backsp },
	{"C-l                      ", "\x0C", redraw },
	{"C-s search               ", "\x13", search },
	{"esc v                    ", "\x1B\x76", pgup },
	{"C-v                      ", "\x16", pgdown },
	{"PgUp                     ", "\x1B\x5B\x35\x7E",pgup }, /* PgUp key */
	{"PgDn                     ", "\x1B\x5B\x36\x7E", pgdown }, /* PgDn key */
	{"esc < beg-of-buf         ", "\x1B\x3C", top },
	{"esc > end-of-buf         ", "\x1B\x3E", bottom },
	{"C-k kill-to-eol          ", "\x0B", killtoeol },
	{"C-x C-s save-buffer      ", "\x18\x13", savebuffer },  
	{"C-x C-c exit             ", "\x18\x03", quit_ask },
	{"K_ERROR                  ", NULL, NULL }
};

char_t *get_key(keymap_t *keys, keymap_t **key_return)
{
	keymap_t *k;
	int submatch;
	static char_t buffer[K_BUFFER_LENGTH];
	static char_t *record = buffer;

	*key_return = NULL;

	/* if recorded bytes remain, return next recorded byte. */
	if (*record != '\0') {
		*key_return = NULL;
		return record++;
	}
	/* reset record buffer. */
	record = buffer;

	do {
		assert(K_BUFFER_LENGTH > record - buffer);
		/* read and record one byte. */
		*record++ = (unsigned)getch();
		*record = '\0';

		/* if recorded bytes match any multi-byte sequence... */
		for (k = keys, submatch = 0; k->key_bytes != NULL; ++k) {
			char_t *p, *q;

			for (p = buffer, q = (char_t *)k->key_bytes; *p == *q; ++p, ++q) {
			        /* an exact match */
				if (*q == '\0' && *p == '\0') {
	    				record = buffer;
					*record = '\0';
					*key_return = k;
					return record; /* empty string */
				}
			}
			/* record bytes match part of a command sequence */
			if (*p == '\0' && *q != '\0') {
				submatch = 1;
			}
		}
	} while (submatch);
	/* nothing matched, return recorded bytes. */
	record = buffer;
	return (record++);
}
