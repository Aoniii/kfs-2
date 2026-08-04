#include "str.h"

int strcmp(const char *s1, const char *s2){
	const u8_t  *t1 = (const u8_t *)s1;
	const u8_t  *t2 = (const u8_t *)s2;

	while (*t1 && *t1 == *t2) {
		t1++;
		t2++;
	}

	return (*t1 - *t2);
}
