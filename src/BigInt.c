#include "BigInt.h"
#include "util.h"
#include "Environment_Internal.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
	result->bytes = toCopy->environment->allocator(result->size);
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
	number->bytes = env->allocator(number->size);	
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
	
	buf = number->environment->allocator((size+1));
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

TR_BigInt* TR_BigInt_absolute(TR_BigInt* operand)
{
	TR_BigInt* result = TR_BigInt_copy(operand);
	result->negative = 0;
	return result;
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
	
	switch (TR_BigInt_compare(TR_BigInt_absolute(operand1),TR_BigInt_absolute(operand2)))
	{
		case 0:
			bytes = operand1->environment->allocator(1);
			bytes[0] = 0;
			result->size = 1;
			result->bytes = bytes;
			return result;
		case 1:
			result->negative = operand1->negative;
			break;
		case -1:
			result->negative = !operand1->negative;
			tmp = operand1;
			operand1 = operand2;
			operand2 = tmp;	
			break;
	}

	k = operand1->size-1;
	bytes = operand1->environment->allocator(operand1->size);

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
	result->bytes = operand1->environment->allocator(size);	
	memcpy(result->bytes,bytes+i,size);
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

	bytes = operand1->environment->allocator(size);
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
	result->bytes = operand1->environment->allocator(size);
	result->size = size;
	memcpy(result->bytes,k==0?bytes+1:bytes,size);
	operand1->environment->deallocator(bytes);

	return result;
}

static TR_BigInt* _multiply_naive(TR_BigInt* operand1, TR_BigInt* operand2)
{
	int i,j,k;
	TR_BigInt* result;
	char* bytes,negative;
	int carry,realsize;

	negative = operand1->negative ^ operand2->negative;
	
	int size = 2*(operand1->size>operand2->size?operand1->size:operand2->size);
	realsize = size;
	bytes = operand1->environment->allocator(size);
	memset(bytes,0,size);


	for (i = operand1->size-1; i >= 0; --i)
	{
		k = (size-1) - ((operand1->size-1) - i);
		carry = 0;
		for (j = operand2->size-1; j >= 0 || carry != 0; --j)
		{
			if (j >= 0)
			{
				carry += (operand1->bytes[i] * operand2->bytes[j]);	
			}
			bytes[k] += carry;
			carry = bytes[k] / 10;
			bytes[k] = bytes[k] % 10;
			if (k < realsize)
			{
				realsize = k;
			}
			--k;	
		}

	}

	realsize = size - realsize;

	result = TR_BigInt_alloc(operand1->environment);
	result->negative = negative;
	result->size = realsize;
	result->bytes = result->environment->allocator(result->size);
	memcpy(result->bytes,bytes+(size-realsize),realsize);
	operand1->environment->deallocator(bytes);
	return result;
}

static TR_BigInt* _canonicalize(TR_BigInt* operand)
{

	TR_BigInt* result;
	int i;
	char *bytes;
	for (i = 0; i < operand->size; ++i)
        {
                if (operand->bytes[i] != 0)
                        break;
        }


	if (i == 0)
	{
		return operand;
	}

	result = TR_BigInt_alloc(operand->environment);

	bytes = operand->environment->allocator(operand->size-i);
	memcpy(bytes,operand->bytes+i,operand->size-i);
	result->bytes = bytes;
	result->negative = operand->negative;
	result->size = operand->size-i;
	
	return result;

	
}

static TR_BigInt* _pad(TR_BigInt* operand, int toSize)
{
	// Caution: this returns a non-canonical representation (which is why it is private)
	TR_BigInt* result = TR_BigInt_alloc(operand->environment);
	result->size = toSize;
	result->negative = operand->negative;
	result->bytes = operand->environment->allocator(toSize);

	if (operand->size >= toSize)
	{
		return operand;
	}

	memset(result->bytes,0,toSize);
	memcpy(result->bytes+(result->size-operand->size),operand->bytes,operand->size);

	return result;		
}

static TR_BigInt* _multiply_karatsuba(TR_BigInt* operand1, TR_BigInt* operand2)
{
	int maxSize,midpoint,expo;
	TR_BigInt *result,*tmp,*highOp1,*highOp2,*lowOp1,*lowOp2,*combined1,*combined2,*int1,*int2,*int3;
	char negative;
	negative = operand1->negative ^ operand2->negative;

	// Equalize operands
	if (operand2->size > operand1->size)
	{
		operand1 = _pad(operand1,operand2->size);
	}
	else if (operand2->size < operand1->size)
	{
		operand2 = _pad(operand2,operand1->size);
	}		

	if (operand1->size < 2 || operand2->size < 2)
	{
		return _multiply_naive(operand1,operand2);
	}
	
	maxSize = operand1->size>operand2->size?operand1->size:operand2->size;
	midpoint = maxSize/2;
	expo = maxSize%2==0?midpoint:midpoint+1;

	if (midpoint > operand1->size || midpoint > operand2->size)
	{
		midpoint = operand1->size<operand2->size?operand1->size-1:operand2->size-1;
	}

	
	TR_Environment* env = operand1->environment;	
	highOp1 = TR_BigInt_alloc(env);
	highOp2 = TR_BigInt_alloc(env);
	lowOp1 = TR_BigInt_alloc(env);
	lowOp2 = TR_BigInt_alloc(env);
	
	highOp1->size = midpoint;
	highOp2->size = midpoint;
	lowOp1->size = operand1->size - midpoint;
	lowOp2->size = operand2->size - midpoint;

	highOp1->bytes = env->allocator(lowOp1->size);
	highOp2->bytes = env->allocator(lowOp2->size);
	lowOp1->bytes = env->allocator(highOp1->size);
	lowOp2->bytes = env->allocator(highOp2->size);

	memcpy(highOp1->bytes,operand1->bytes,midpoint);
	memcpy(highOp2->bytes,operand2->bytes,midpoint);
	memcpy(lowOp1->bytes,operand1->bytes+(midpoint),lowOp1->size);
	memcpy(lowOp2->bytes,operand2->bytes+(midpoint),lowOp2->size);


	combined1 = TR_BigInt_add(lowOp1,highOp1);
	combined2 = TR_BigInt_add(lowOp2,highOp2);

	int1 = _multiply_karatsuba(lowOp1,lowOp2);
	int2 = _multiply_karatsuba(combined1,combined2);
	int3 = _multiply_karatsuba(highOp1,highOp2);

	char* buf = env->allocator(midpoint*2);
	memset(buf,0,midpoint*10);
	snprintf(buf,midpoint*10,"%d",(int)pow(10,2*expo));
	tmp = TR_BigInt_fromString(env,buf);
	result = _multiply_naive(int3,tmp);
		

	memset(buf,0,midpoint*10);
	snprintf(buf,midpoint*10,"%d",(int)pow(10,expo));
	tmp = TR_BigInt_fromString(env,buf);
	combined1 = TR_BigInt_subtract(int2,int3);
	combined2 = TR_BigInt_subtract(combined1,int1);
	combined1 = _multiply_naive(combined2,tmp);

	TR_BigInt* tr = result;
	result = TR_BigInt_add(result,combined1);
	
	TR_BigInt* tt = result;
	result = TR_BigInt_add(result,int1);

	result->negative = negative;
	return _canonicalize(result);
}

TR_BigInt* TR_BigInt_multiply(TR_BigInt* operand1, TR_BigInt* operand2)
{

	// ~ 12 is when Karatsuba's exceeds performance of naive multiplication
	//if (operand1->size > 11 || operand2->size > 11)
	{
		return _multiply_karatsuba(operand1,operand2);
	}

	return _multiply_naive(operand1,operand2);
}


 


