#include "put.h"
#include "shell.h"

void    halt(void) {
    putstr("System halted.\n");
    __asm__ volatile ("cli");       /* Clear Interrupt flag */
    for (;;)
        __asm__ volatile ("hlt");   /* Halt */
}
