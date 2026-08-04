/* Port-mapped I/O helpers (x86 in/out). */
#ifndef KFS_IO_H
#define KFS_IO_H

#include "types.h"

static inline void  outb(u16_t port, u8_t val)
{
	__asm__ volatile ("out %1, %0" : : "a"(val), "Nd"(port));
}

static inline u8_t  inb(u16_t port)
{
	u8_t ret = 0;

	__asm__ volatile ("in %0, %1" : "=a"(ret) : "Nd"(port));
	return (ret);
}

#endif /* KFS_IO_H */
