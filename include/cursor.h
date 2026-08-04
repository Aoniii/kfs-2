/* Hardware VGA cursor control. */
#ifndef KFS_CURSOR_H
#define KFS_CURSOR_H

#include "types.h"

u16_t   get_cursor_pos(void);
void    enable_cursor(u8_t start, u8_t end);
void    disable_cursor(void);
void    update_cursor(size_t x, size_t y);

#endif /* KFS_CURSOR_H */
