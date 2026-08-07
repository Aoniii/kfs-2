#include "debug.h"
#include "put.h"
#include "types.h"

/* Dump 256 bytes of the current stack as hex + ASCII, for low-level debugging. */
void    stack_dump(void) {
    u8_t    *esp;

    __asm__ volatile ("mov %0, esp" : "=r"(esp));

    for (size_t offset = 0; offset < 256; offset += 16) {
        putptr((ptr_t)esp + offset, FALSE);

        for (size_t i = 0; i < 16; i++) {
            putchar(' ');
            if (i % 8 == 0)
                putchar(' ');
            putbyte(esp[offset + i], TRUE);
        }

        putstr("  |");

        for (size_t i = 0; i < 16; i++) {
            if (esp[offset + i] < 33 || esp[offset + i] > 126)
                putchar('.');
            else
                putchar(esp[offset + i]);
        }

        putstr("|\n");
    }
}
