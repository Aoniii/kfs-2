#ifndef KFS_DEBUG_H
#define KFS_DEBUG_H

/* Hexdump-style dump of the kernel stack around ESP. */
void    stack_dump(void);

/* Print the current GDTR (base/limit) and segment registers. */
void    gdt_debug(void);

/* Dump the general-purpose registers (snapshot at the moment of reading). */
void    dump_registers(void);

#endif /* KFS_DEBUG_H */
