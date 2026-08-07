#include "shell.h"
#include "put.h"

void    shell_run(void) {
    char    buf[256];

    while (1) {
        read_line(buf, sizeof(buf));
        putstr("kfs>");
    }
}
