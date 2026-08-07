/* Minimal in-kernel debug shell. */
#ifndef KFS_SHELL_H
#define KFS_SHELL_H

#include "types.h"

/* Read one line into buffer (bounded editing). Returns its length. */
size_t  read_line(char *buffer, size_t size);

/* Run the interactive shell loop: prompt, read a line, dispatch commands. */
void    shell_run(void);

#endif /* KFS_SHELL_H */
