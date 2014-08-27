#include "BigInt.h"
#include "util.h"
#include "Environment_Internal.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct TR_BigInt
{
	char *bytes;
	char negative;
	int size;
	TR_Environment* environment;
};

TR_BigInt* TR_BigInt_alloc(TR_Environment* env)
{
	TR_BigInt* result = env->allocator(sizeof(TR_BigInt));
	result->size = 0;
	result->bytes = NULL;
	result->negative = 0;
	result->environment = env;
	return result;
}

void TR_BigInt_free(TR_BigInt *number)
{
	if (number->bytes != NULL)
	{
		number->environment->deallocator(number->bytes);
	}
	number->environment->deallocator(number);
}

TR_BigInt* TR_BigInt_copy(TR_BigInt *toCopy)
{
	TR_BigInt* result = TR_BigInt_alloc(toCopy->environment);
	result->negative = toCopy->negative; 
	result->size = toCopy->size;
	result->bytes = toCopy->environment->allocator(sizeof(char)*result->size);
	memcpy(result->bytes,toCopy->bytes,toCopy->size);

	return result;
}

TR_BigInt* TR_BigInt_fromString(TR_Environment* env,const char* str)
{
	const char* cur = str;
	int i = 0;
	TR_BigInt *number = TR_BigInt_alloc(env);

	number->negative = 0;
	if (*cur == '-')
	{
		number->negative = 1;
		cur++;
	}

	number->size = strlen(cur);
	number->bytes = env->allocator(sizeof(char)*number->size);	
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
	
	buf = number->environment->allocator(sizeof(char)*(size+1));
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
	buf[i] = '\0';

	return origin;
}

int TR_BigInt_compare(TR_BigInt *operand1, TR_BigInt *operand2)
{
	int i;
	char digit1,digit2;
	if (operand1->negative ^ operand2->negative)
	{
		return operand1->negative?-1:1;
	}

	if (operand1->size > operand2->size)
	{
		return operand1->negative?-1:1;
	}

	if (operand1->size < operand2->size)
	{
		return operand2->negative?1:-1;
	}

	for (i = operand1->size-1; i >= 0; --i)
	{
		digit1 = operand1->bytes[i];
		digit2 = operand2->bytes[i];

		if (digit1 > digit2)
		{
			return operand1->negative?-1:1;
		}
		if (digit2 < digit2)
		{
			return operand2->negative?1:-1;
		}	
	}	

	return 0;
}

char TR_BigInt_greaterThan(TR_BigInt *operand1,TR_BigInt *operand2)
{
	return TR_BigInt_compare(operand1,operand2)==1?1:0;
}

char TR_BigInt_lessThan(TR_BigInt *operand1,TR_BigInt *operand2)
{
	return TR_BigInt_compare(operand1,operand2)==-1?1:0;
}

char TR_BigInt_equal(TR_BigInt *operand1,TR_BigInt *operand2)
{
	return TR_BigInt_compare(operand1,operand2)==0?1:0;
}

TR_BigInt* TR_BigInt_subtract(TR_BigInt *operand1, TR_BigInt *operand2)
{
	int i,j,k,size;
	TR_BigInt* tmp;
	TR_BigInt* result = TR_BigInt_alloc(operand1->environment);
	char* bytes, digit1,digit2,carry = 0;	
	
	switch (TR_BigInt_compare(operand1,operand2))
	{
		case 0:
			bytes = operand1->environment->allocator(sizeof(char));
			bytes[0] = 0;
			result->size = 1;
			result->bytes = bytes;
			return result;
		case 1:
			result->negative = 0;
			break;
		case -1:
			result->negative = 1;
			break;
	}

	switch (operand1->size > operand2->size)
	{

		case 0:
			tmp = operand1;
			operand1 = operand2;
			operand2 = tmp;	
			break;
		case 1:
			break;
	}
	k = operand1->size-1;
	bytes = operand1->environment->allocator(sizeof(char)*operand1->size);

	for (i = operand1->size-1,k = i,j = operand2->size-1; i >= 0; --i,--j,--k)
	{
		digit1 = operand1->bytes[i];
		digit2 = j >= 0?operand2->bytes[j]:0;
		if (digit1 < digit2 + carry)
		{
			bytes[k] = 10 - (digit2 + carry - digit1);
			carry = 1;
		}
		else
		{	
			bytes[k] = digit1 - digit2 - carry;
			carry = 0;
		}
	}
	for (i = 0; i < operand1->size; ++i)
	{
		if (bytes[i] != 0)
			break;
	}
	size = operand1->size - i;
	result->bytes = operand1->environment->allocator(sizeof(char)*size);	
	memcpy(result->bytes,bytes,size);
	result->size = size;
	operand1->environment->deallocator(bytes);
	return result;	
}

TR_BigInt* TR_BigInt_add(TR_BigInt *operand1, TR_BigInt *operand2)
{
	int i,j,k,size;
	TR_BigInt* result = TR_BigInt_alloc(operand1->environment);
	char subtract = operand1->negative ^ operand2->negative;
	char digit1,digit2,carry;
	size = (MAX(operand1->size,operand2->size)+1);
	char* bytes;

	bytes = operand1->environment->allocator(sizeof(char)*size);
	k = size-1;

	

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

	result->negative = (operand1->negative && operand2->negative);
	result->bytes = operand1->environment->allocator(sizeof(char)*size);
	result->size = size;
	memcpy(result->bytes,k==0?bytes+1:bytes,size);
	operand1->environment->deallocator(bytes);

	return result;
}


 


