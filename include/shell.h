/* Minimal in-kernel debug shell. */
#ifndef KFS_SHELL_H
#define KFS_SHELL_H

#define LINE_MAX 128

/* Run the interactive shell loop: per-TTY prompt, read input, dispatch. */
void    shell_run(void);

#endif /* KFS_SHELL_H */
