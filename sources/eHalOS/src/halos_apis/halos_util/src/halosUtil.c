#include <inttypes.h>
#include "../inc/halosUtil.h"

#define MAX_INT_CHARS 15


char* halos_itoa(int32_t i) {
	static char a[MAX_INT_CHARS];

	char *b = a + sizeof(a) - 1;
	uint8_t   sign = (i < 0);

	if (sign) i = -i;
	*b = 0;
	do {
		*--b = '0' + (i % 10);
		i /= 10;
	} while (i);
	if (sign) *--b = '-';
	return b;
}


char* halos_itoa_base(int32_t val, int32_t base){

	static char buf[32] = {0};

	int32_t i = 30;

	for(; val && i ; --i, val /= base)

		buf[i] = "0123456789abcdef"[val % base];

	return &buf[i+1];
}
