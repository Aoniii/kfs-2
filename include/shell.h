/* Minimal in-kernel debug shell. */
#ifndef KFS_SHELL_H
#define KFS_SHELL_H

#define LINE_MAX 128

typedef struct {
    char    *cmd;
    void    (*f)(void);
}       t_cmd;

/* Run the interactive shell loop: per-TTY prompt, read input, dispatch. */
void    shell_run(void);

/* Method for reading the list of commands and executing the associated functions */
void    dispatch(const char *cmd);

#endif /* KFS_SHELL_H */
