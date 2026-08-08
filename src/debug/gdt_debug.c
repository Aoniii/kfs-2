#include "debug.h"
#include "gdt.h"
#include "fmt.h"

void    gdt_debug(void) {
    gdt_ptr_t   cur;
    u16_t       cs, ds, ss;

    __asm__ volatile ("sgdt %0" : "=m"(cur));      // sgdt = reverse of lgdt
    __asm__ volatile ("mov %0, cs" : "=r"(cs));
    __asm__ volatile ("mov %0, ds" : "=r"(ds));
    __asm__ volatile ("mov %0, ss" : "=r"(ss));

    printk("GDTR base=%x limit=%x\n", cur.base, (u32_t)cur.limit);
    printk("CS=%x DS=%x SS=%x\n", (u32_t)cs, (u32_t)ds, (u32_t)ss);
}
