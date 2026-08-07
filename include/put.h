/* Low-level output primitives (write directly to the terminal). */
#ifndef KFS_PUT_H
#define KFS_PUT_H

#include "types.h"

void    putbyte(u8_t b, bool_t lower);
void    putchar(char c);
void    puthex(u32_t n, bool_t lower, bool_t prefix);
void    putnbr(int n);
void    putptr(ptr_t p, bool_t prefix);
void    putstr(const char *s);

#endif /* KFS_PUT_H */
