#include "gdt.h"

/* The value handed to `lgdt` (its address is passed to gdt_flush). */
static gdt_ptr_t    gdt_ptr;

/*
 * Fill one descriptor, spreading base/limit across the scattered fields.
 *
 * access byte  =  P | DPL | S | E | DC | RW | A
 *   P   (bit7) present            -> 1
 *   DPL (6-5) ring                -> 00 kernel / 11 user
 *   S   (bit4) code/data segment  -> 1
 *   E   (bit3) executable         -> 1 code / 0 data
 *   RW  (bit1) readable / writable
 *   => 0x9A kernel code, 0x92 kernel data, 0xFA user code, 0xF2 user data
 *
 * gran byte  (only the high nibble is used here):
 *   G  (bit7) granularity, limit in 4K pages -> 1
 *   DB (bit6) 32-bit segment                 -> 1
 *   => 0xCF gives a 4 GiB flat segment
 */
static void gdt_set_entry(gdt_entry_t *gdt, int i, u32_t base, u32_t limit, u8_t access, u8_t gran) {
	gdt[i].limit_low    = (limit & 0xFFFF);
	gdt[i].base_low     = (base & 0xFFFF);
	gdt[i].base_mid     = (base >> 16) & 0xFF;
	gdt[i].access       = access;
	gdt[i].flags_limit  = ((limit >> 16) & 0x0F) | (gran & 0xF0);
	gdt[i].base_high    = (base >> 24) & 0xFF;
}

void    gdt_init(void) {
    /* The GDT lives at the fixed address required by the subject (0x800). */
	gdt_entry_t *gdt = (gdt_entry_t *)GDT_ADDRESS;

	/*
	 * Optimization barrier: hide the constant address from the compiler so
	 * its -Warray-bounds analysis doesn't mistake 0x800 for a near-null
	 * object (false positive on a fixed low address).
	 */
	__asm__ volatile ("" : "+r"(gdt));

	/*              idx     base    limit   access  gran    */
	gdt_set_entry(gdt,  0,  0,  0,          0,      0);     /* null             */
	gdt_set_entry(gdt,  1,  0,  0xFFFFFFFF, 0x9A,   0xCF);  /* kernel code      */
	gdt_set_entry(gdt,  2,  0,  0xFFFFFFFF, 0x92,   0xCF);  /* kernel data      */
	gdt_set_entry(gdt,  3,  0,  0xFFFFFFFF, 0x92,   0xCF);  /* kernel stack     */
	gdt_set_entry(gdt,  4,  0,  0xFFFFFFFF, 0xFA,   0xCF);  /* user code        */
	gdt_set_entry(gdt,  5,  0,  0xFFFFFFFF, 0xF2,   0xCF);  /* user data        */
	gdt_set_entry(gdt,  6,  0,  0xFFFFFFFF, 0xF2,   0xCF);  /* user stack       */

	gdt_ptr.limit = (sizeof(gdt_entry_t) * GDT_ENTRIES) - 1;
	gdt_ptr.base  = GDT_ADDRESS;

	/* Load the GDTR and reload the segment registers (asm). */
	gdt_flush((u32_t)&gdt_ptr);
}
