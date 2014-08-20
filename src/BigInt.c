#include "BigInt.h"
#include <string.h>
#include <stdlib.h>

struct TR_BigInt
{
	char *bytes;
	int size;
};

TR_BigInt* TR_BigInt_alloc()
{
	return malloc(sizeof(TR_BigInt));
}

void TR_BigInt_free(TR_BigInt *number)
{
	free(number);
}

TR_BigInt* TR_BigInt_fromString(const char* str)
{
	const char* cur = str;
	int i = 0;
	TR_BigInt *number = TR_bigint_alloc();
	number->size = strlen(str);
	number->bytes = malloc(sizeof(char)*number->size);	
	while (*cur != '\0')
	{
		number->bytes[i++] = *(cur++) - '0';	
	}

	return number;	
}

const char* TR_BigInt_toString(TR_BigInt *number)
{
	char* buf = malloc(sizeof(char)*number->size);
	int i = 0;
	while (i < number->size)
	{
		buf[i] = number->bytes[i] + '0';
		++i;
	}
	return buf;
}

