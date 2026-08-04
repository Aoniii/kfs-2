#include "str.h"

size_t  strlen(const char *s) {
	char *tmp = (char *)s;

	while (*tmp)
		tmp++;

	return (tmp - s);
}
