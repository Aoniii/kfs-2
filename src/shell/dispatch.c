#include "debug.h"
#include "shell.h"
#include "types.h"
#include "str.h"
#include "vga.h"

/* List of commands, with the strings associated with each command's function */
static const t_cmd  cmd_list[] = {
    {"stack", &stack_dump},
    {"reboot", &reboot},
    {"halt", &halt},
    {"clear", &terminal_clear},
    {NULL, NULL}    /* sentinel */
};

/* Method for reading the list of commands and executing the associated functions */
void    dispatch(const char *cmd) {
    u8_t    i;

    if (cmd[0] == '\0')
        return ;        /* empty line -> nothing */

    i = 0;
    while (cmd_list[i].cmd) {
        if (strcmp(cmd, cmd_list[i].cmd) == 0) {
            cmd_list[i].f();
            break ;
        }
        i++;
    }
}
