#include "debug.h"
#include "fmt.h"
#include "types.h"

/* Dump the general-purpose registers (snapshot at the moment of reading). */
void    dump_registers(void) {
	u32_t   eax, ebx, ecx, edx, esi, edi, esp, ebp, eflags;

	__asm__ volatile ("mov %0, eax" : "=r"(eax));
	__asm__ volatile ("mov %0, ebx" : "=r"(ebx));
	__asm__ volatile ("mov %0, ecx" : "=r"(ecx));
	__asm__ volatile ("mov %0, edx" : "=r"(edx));
	__asm__ volatile ("mov %0, esi" : "=r"(esi));
	__asm__ volatile ("mov %0, edi" : "=r"(edi));
	__asm__ volatile ("mov %0, esp" : "=r"(esp));
	__asm__ volatile ("mov %0, ebp" : "=r"(ebp));
	__asm__ volatile ("pushfd ; pop %0" : "=r"(eflags));

	printk("eax=%x ebx=%x ecx=%x edx=%x\n", eax, ebx, ecx, edx);
	printk("esi=%x edi=%x esp=%x ebp=%x\n", esi, edi, esp, ebp);
	printk("eflags=%x\n", eflags);
}
