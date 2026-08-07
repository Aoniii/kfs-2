#include "put.h"
#include "types.h"

void    putbyte(u8_t b, bool_t lower) {
    const char  *base = lower ? "0123456789abcdef" : "0123456789ABCDEF";

    putchar(base[(b >> 4) & 0xF]);
    putchar(base[b & 0xF]);
}
