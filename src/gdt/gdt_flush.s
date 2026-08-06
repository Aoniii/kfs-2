.intel_syntax noprefix

/*
 * void gdt_flush(u32_t gdt_ptr);
 *
 * gdt_ptr (on the stack at [esp + 4]) is the address of the 6-byte gdt_ptr_t.
 * We load the GDTR with it, then reload every segment register so the CPU
 * starts using OUR descriptors instead of the ones GRUB left cached.
 */
.section .text
.global gdt_flush
.type gdt_flush, @function
gdt_flush:
    mov eax, [esp + 4]      /* eax = &gdt_ptr */
    lgdt [eax]              /* load GDTR: "the GDT is at 0x800, 56 bytes"    */

    /*
	 * Reload the data & stack segment registers with the kernel data
	 * selector (0x10). These ones accept a plain mov.
	 */
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

    /*
	 * CS cannot be loaded with mov. A far jump sets CS to the kernel code
	 * selector (0x08) and makes the CPU re-read our GDT for it.
	 */
    ljmp 0x08, OFFSET .flush

.flush:
    ret

.size gdt_flush, . - gdt_flush
