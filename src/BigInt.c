#include "BigInt.h"
#include "util.h"
#include <string.h>
#include <stdlib.h>

struct TR_BigInt
{
	char *bytes;
	char negative;
	int size;
};

TR_BigInt* TR_BigInt_alloc()
{
	TR_BigInt* result = malloc(sizeof(TR_BigInt));
	result->size = 0;
	result->bytes = NULL;
	result->negative = 0;
}

void TR_BigInt_free(TR_BigInt *number)
{
	if (number->bytes != NULL)
	{
		free(number->bytes);
	}
	free(number);
}

TR_BigInt* TR_BigInt_copy(TR_BigInt *toCopy)
{
	TR_BigInt* result = TR_BigInt_alloc();
	result->negative = toCopy->negative; 
	result->size = toCopy->size;
	result->bytes = malloc(sizeof(char)*result->size);
	memcpy(result->bytes,toCopy->bytes,toCopy->size);

	return result;
}

TR_BigInt* TR_BigInt_fromString(const char* str)
{
	const char* cur = str;
	int i = 0;
	TR_BigInt *number = TR_BigInt_alloc();

	number->negative = 0;
	if (*cur == '-')
	{
		number->negative = 1;
		cur++;
	}

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
	int size = number->size;
	char *buf,*origin;
	int i = 0;

	if (number->negative)
		size++;
	
	buf = malloc(sizeof(char)*size);
	origin = buf;
	if (number->negative)
	{
		buf[0] = '-';		
		buf++;
	}
	
	while (i < number->size)
	{
		buf[i] = number->bytes[i] + '0';
		++i;
	}

	return origin;
}

TR_BigInt* TR_BigInt_subtract(TR_BigInt *operand1, TR_BigInt *operand2)
{
	TR_BigInt* tmp = TR_BigInt_copy(operand2);
	tmp->negative = !tmp->negative;
	TR_BigInt* result = TR_BigInt_add(operand1,tmp);
	TR_BigInt_free(tmp);
	return result;
}

TR_BigInt* TR_BigInt_add(TR_BigInt *operand1, TR_BigInt *operand2)
{
	int i,j,k,size;
	TR_BigInt* result = TR_BigInt_alloc();
	char subtract = operand1->negative ^ operand2->negative;
	char digit1,digit2,carry;
	size = (MAX(operand1->size,operand2->size)+1);

	char* bytes = malloc(sizeof(char)*size);
	k = size-1;

	if (subtract)
	{
		// TODO: implement subtraction
		return NULL;
	}

	for (i=operand1->size-1,j=operand2->size-1,carry=0;i >= 0 || j >= 0 || carry > 0;)
	{
		digit1 = i>=0?operand1->bytes[i--]:0;
		digit2 = j>=0?operand2->bytes[j--]:0;
		carry += digit1+digit2;
		bytes[k--] = carry % 10;
		carry /= 10;			
	}

	if (k==0)
		size-=1;

	result->negative = operand1->negative && operand2->negative;
	result->bytes = malloc(sizeof(char)*size);
	result->size = size;
	memcpy(result->bytes,k==0?bytes+1:bytes,size);
	free(bytes);

	return result;
}
