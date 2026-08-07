#include "put.h"

void    putptr(ptr_t p, bool_t prefix) {
	puthex((u32_t)p, TRUE, prefix);
}
