#include "io.h"
#include "shell.h"

void    reboot(void) {
    /* Pulse the CPU reset line via the 8042 keyboard controller. */
    outb(0x64, 0xFE);
}
