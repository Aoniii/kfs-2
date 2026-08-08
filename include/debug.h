#ifndef KFS_DEBUG_H
#define KFS_DEBUG_H

/* Hexdump-style dump of the kernel stack around ESP. */
void    stack_dump(void);

/* Print the current GDTR (base/limit) and segment registers. */
void    gdt_debug(void);

#endif /* KFS_DEBUG_H */
