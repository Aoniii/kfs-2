#include "shell.h"
#include "fmt.h"
#include "keyboard.h"
#include "vga.h"
#include "types.h"

static char     line[TTY_COUNT][LINE_MAX];
static size_t   line_len[TTY_COUNT];

static void prompt(void) {
    printk("kfs (%d)>", tty_get_current());
}

/* Feed one character into the CURRENT tty's input line. */
static void feed(size_t tty, char c) {
    if (c == '\n') {
        terminal_putchar('\n');
        line[tty][line_len[tty]] = '\0';
        dispatch(line[tty]);
        line_len[tty] = 0;
        prompt();
    } else if (c == '\b') {
        if (line_len[tty] > 0) {
            line_len[tty]--;
            terminal_putchar('\b');
        }
    } else {
        if (line_len[tty] + 1 < LINE_MAX) {
            line[tty][line_len[tty]] = c;
            line_len[tty]++;
            terminal_putchar(c);
        }
    }
}

void    shell_run(void) {
    size_t  t;

    /* Give every TTY its own prompt so each looks like an independent shell. */
	for (t = 0; t < TTY_COUNT; t++) {
		tty_switch(t);
		line_len[t] = 0;
		prompt();
	}
	tty_switch(0);

    for (;;) {
        char c = keyboard_getchar();
        feed(tty_get_current(), c);
    }
}
