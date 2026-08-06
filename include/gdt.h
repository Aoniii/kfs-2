/* Global Descriptor Table (GDT): defines the memory segments the CPU uses. */
#ifndef KFS_GDT_H
#define KFS_GDT_H

#include "types.h"

/* Where the GDT must live (42 requirement) and how many entries it holds. */
#define GDT_ADDRESS     0x00000800
#define GDT_ENTRIES     7

/* Segment selectors = index * 8 (the low 3 bits are TI + RPL, so 8-aligned). */
#define GDT_KERNEL_CODE     0x08    /* entry 1 */
#define GDT_KERNEL_DATA     0x10    /* entry 2 */
#define GDT_KERNEL_STACK    0x18    /* entry 3 */
#define GDT_USER_CODE       0x20    /* entry 4 */
#define GDT_USER_DATA       0x28    /* entry 5 */
#define GDT_USER_STACK      0x30    /* entry 6 */

/* One 8-byte segment descriptor.
   The base and limit are split across several fields for historical reasons
   (the 80286 -> 80386 extension), which is why it looks scattered. */
typedef struct {
	u16_t   limit_low;      /* limit  bits 0-15                                  */
	u16_t   base_low;       /* base   bits 0-15                                  */
	u8_t    base_mid;       /* base   bits 16-23                                 */
	u8_t    access;         /* P | DPL | S | E | DC | RW | A                     */
	u8_t    flags_limit;    /* flags (G,DB,L,AVL) bits 4-7 | limit bits 16-19    */
	u8_t    base_high;      /* base   bits 24-31                                 */
} __attribute__((packed)) gdt_entry_t;

/* The 6-byte value loaded into the GDTR register by `lgdt`. */
typedef struct {
	u16_t   limit;          /* size of the GDT in bytes, minus 1                 */
	u32_t   base;           /* linear address of the GDT (here: GDT_ADDRESS)     */
} __attribute__((packed)) gdt_ptr_t;

/* Build the GDT at GDT_ADDRESS and load it (called early from kernel_main). */
void    gdt_init(void);

/* Print the current GDTR (base/limit) and segment registers, for debugging. */
void    gdt_debug(void);

/* ASM routine (src/gdt/gdt_flush.s): lgdt + reload segment registers.
   Takes the address of the gdt_ptr_t to load. */
extern void gdt_flush(u32_t gdt_ptr);

#endif /* KFS_GDT_H */

