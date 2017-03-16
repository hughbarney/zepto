/* key.c, Zepto Emacs, Public Domain, Hugh Barney, 2017, Derived from: Anthony's Editor January 93 */

#include "header.h"

/* desc, keys, func */
keymap_t keymap[] = {
	{"C-a beginning-of-line    ", "\x01", lnbegin },
	{"C-b                      ", "\x02", left },
	{"C-d forward-delete-char  ", "\x04", delete },
	{"C-e end-of-line          ", "\x05", lnend },
	{"C-f                      ", "\x06", right },
	{"C-n                      ", "\x0E", down },
	{"C-p                      ", "\x10", up },
	{"C-h backspace            ", "\x08", backsp },
	{"C-k kill-to-eol          ", "\x0B", killtoeol },
	{"C-s search               ", "\x13", search },
	{"C-v                      ", "\x16", pgdown },
	{"C-w kill-region          ", "\x17", cut},
	{"C-y yank                 ", "\x19", paste},
	{"C-space set-mark         ", "\x00", set_mark },
	{"esc @ set-mark           ", "\x1B\x40", set_mark },
	{"esc k kill-region        ", "\x1B\x6B", cut },
	{"esc v                    ", "\x1B\x76", pgup },
	{"esc w copy-region        ", "\x1B\x77", copy},
	{"esc < beg-of-buf         ", "\x1B\x3C", top },
	{"esc > end-of-buf         ", "\x1B\x3E", bottom },
	{"up previous-line         ", "\x1B\x5B\x41", up },
	{"down next-line           ", "\x1B\x5B\x42", down },
	{"left backward-character  ", "\x1B\x5B\x44", left },
	{"right forward-character  ", "\x1B\x5B\x43", right },
	{"home beginning-of-line   ", "\x1B\x4F\x48", lnbegin },
	{"end end-of-line          ", "\x1B\x4F\x46", lnend },
	{"DEL forward-delete-char  ", "\x1B\x5B\x33\x7E", delete },
	{"backspace delete-left    ", "\x7f", backsp },
	{"PgUp                     ", "\x1B\x5B\x35\x7E",pgup },
	{"PgDn                     ", "\x1B\x5B\x36\x7E", pgdown },
	{"C-x C-s save-buffer      ", "\x18\x13", save },  
	{"C-x C-c exit             ", "\x18\x03", quit },
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
