/* Umbrella header: pulls in the whole kernel API.
   Include this from a .c that needs "everything"; otherwise prefer the
   specific module headers (vga.h, str.h, ...) for tighter dependencies. */
#ifndef KFS_KERNEL_H
#define KFS_KERNEL_H

#if defined(__linux__)
# error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
# error "This kernel must be compiled with a i386/i686 (32-bit) compiler"
#endif

#include "types.h"
#include "io.h"
#include "str.h"
#include "put.h"
#include "fmt.h"
#include "vga.h"
#include "cursor.h"
#include "keyboard.h"
#include "gdt.h"

void    kernel_main(void);

#endif /* KFS_KERNEL_H */
