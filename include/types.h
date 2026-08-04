/* Fundamental types and constants shared across the kernel. */
#ifndef KFS_TYPES_H
#define KFS_TYPES_H

#define NULL ((void *)0)

typedef unsigned long   size_t;
typedef unsigned long   u64_t;
typedef unsigned int    u32_t;
typedef unsigned short  u16_t;
typedef unsigned char   u8_t;
typedef unsigned long   ptr_t;

#define PTR_SIZE (sizeof(void *))

typedef enum
{
	FALSE,
	TRUE
}   bool_t;

#endif /* KFS_TYPES_H */
