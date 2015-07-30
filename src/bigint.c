#include "trooth/bigint.h"
#include "trooth/util_internal.h"
#include "trooth/environment_internal.h"
#include "trooth/bigint_internal.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


/*****************/
/* Alloc/dealloc */
/*****************/
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




/*********************/
/* Utility Functions */
/*********************/

/* All of the below util functions operate 'in place' for efficiency */
static void _pad(TR_BigInt* operand, int toSize)
{
	TR_BigInt* result;
	char* bytes;

	if (operand->size < toSize)
	{
		bytes = operand->environment->allocator(toSize);

		memset(bytes,0,toSize);
		memcpy(bytes+(toSize-operand->size),operand->bytes,operand->size);

		operand->environment->deallocator(operand->bytes);
		operand->size = toSize;
		operand->bytes = bytes;
	}

}


static void _canonicalize(TR_BigInt* operand)
{
	int i;
	char *bytes;

	/* Only one zero representation */
	if (operand->size == 1 && operand->bytes[0] == 0)
	{
	  operand->negative = 0;
	  return;
	}

	/* no leading zeros */
	for (i = 0; i < operand->size; ++i)
        {
                if (operand->bytes[i] != 0)
                        break;
        }

	if (i == 0)
	{
		return;
	}

	bytes = operand->environment->allocator(operand->size-i);
	memcpy(bytes,operand->bytes+i,operand->size-i);
	operand->environment->deallocator(operand->bytes);
	operand->bytes = bytes;
	operand->size = operand->size-i;

}




/****************/
/* Input/Output */
/****************/
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

char* TR_BigInt_toString(TR_BigInt *number)
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

/*************************/
/* Comparison/Processing */
/*************************/
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

	for (i = 0; i < operand1->size; ++i)
	{
		digit1 = operand1->bytes[i];
		digit2 = operand2->bytes[i];
		if (digit1 > digit2)
		{
			return operand1->negative?-1:1;
		}
		if (digit1 < digit2)
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


/**************/
/* Operations */
/**************/

TR_BigInt* TR_BigInt_absolute(TR_BigInt* operand)
{
	TR_BigInt* result = TR_BigInt_copy(operand);
	result->negative = 0;
	return result;
}


TR_BigInt* TR_BigInt_subtract(TR_BigInt *operand1, TR_BigInt *operand2)
{
	int i,j,k,size;
	TR_BigInt* tmp;
	TR_BigInt *absVal, *tmpVal;
	TR_BigInt* result = TR_BigInt_alloc(operand1->environment);
	char* bytes,digit1,digit2,carry = 0;

	if (operand1->negative ^ operand2->negative)
	{
			absVal = TR_BigInt_copy(operand2);
			absVal->negative = operand1->negative;
			tmpVal = TR_BigInt_add(operand1,absVal);
			TR_BigInt_free(absVal);
			return tmpVal;

	}

	TR_BigInt* op1_absolute = TR_BigInt_absolute(operand1);
	TR_BigInt* op2_absolute = TR_BigInt_absolute(operand2);

	switch (TR_BigInt_compare(op1_absolute,op2_absolute))
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
			result->negative = !(operand2->negative);
			tmp = operand1;
			operand1 = operand2;
			operand2 = tmp;
			break;
	}
	TR_BigInt_free(op1_absolute);
	TR_BigInt_free(op2_absolute);

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
	//operand1->environment->deallocator(bytes);
	return result;
}


TR_BigInt* TR_BigInt_add(TR_BigInt *operand1, TR_BigInt *operand2)
{
	int i,j,k,size;
	TR_BigInt* result;
	TR_BigInt *neg,*pos,*absVal,*tmpVal;
	char subtract = operand1->negative ^ operand2->negative;
	char digit1,digit2,carry;
	char* bytes;

	/* Handle if this is actually subtraction */
	if (subtract)
	{
		neg = operand1->negative?operand1:operand2;
		pos = operand1->negative?operand2:operand1;
		absVal = TR_BigInt_absolute(neg);
		tmpVal = TR_BigInt_subtract(pos,absVal);
		TR_BigInt_free(absVal);
		return tmpVal;
	}

	result = TR_BigInt_alloc(operand1->environment);

	/* Max size cannot exceed 1 + max numeral size */
	size = MAX(operand1->size,operand2->size);
	size += 1;

	bytes = operand1->environment->allocator(size);
	memset(bytes,0,size);
	k = size-1;



	for (i=operand1->size-1,j=operand2->size-1,carry=0;i >= 0 || j >= 0 || carry > 0;)
	{
		digit1 = i>=0?operand1->bytes[i--]:0;
		digit2 = j>=0?operand2->bytes[j--]:0;
		carry += digit1+digit2;
		bytes[k--] = carry % 10;
		carry /= 10;
	}


	result->negative = (operand1->negative && operand2->negative);
	result->bytes = operand1->environment->allocator(size);
	result->size = size;

	memcpy(result->bytes,bytes,size);

	operand1->environment->deallocator(bytes);

	_canonicalize(result);
	return result;
}

/**
 * Performs naive multiplication algorithm
 * O(n^2) - will return result and is non-destructive
 */
static TR_BigInt* _multiply_naive(TR_BigInt* operand1, TR_BigInt* operand2)
{
	int i,j,k;
	TR_BigInt* result;
	char* bytes,negative;
	int carry,realsize;

	negative = operand1->negative ^ operand2->negative;

	/* The size of the result should never exceed 2 * max of sizes
	 * or to put another way, if you multiple a number by itself, the
	 * worst case is the number by itself, and worst case of this is a
	 * number of form (10^n)-1 - where n is number of digits - our claim
	 * is that in that case, we can guarantee result is 2n or fewer digits
	 */
	int size = 2*MAX(operand1->size,operand2->size);

	/* We will use realsize to track the actual size of result */
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
	_canonicalize(result);
	return result;
}

/* A safe power function for use in karatsuba - makes assumptions therein
 * so should not be used for any other power operations */
static TR_BigInt* _karatsuba_safe_power(TR_Environment* env,int expo)
{
	// Is safe to use int for expo since it is used for size, it won't overflow
	TR_BigInt* tmp;
	char* result = env->allocator(expo+1);
	memset(result,'0',expo+1);
	result[0] = '1';
	result[expo+1] = '\0';
	tmp = TR_BigInt_fromString(env,result);
	env->deallocator(result);
	return tmp;

}

/* Performs a karatsuba multiplication on inputs, falls back
 * to naive multiplication as the base case, will automatically
 * pad inputs correctly to apply algorithm
 *
 * At some point we should provide a proof for this since the
 * "how is a clock accurate if you only measure it by other clocks"
 * argument has some credence for more complex algorithms like this
 */
static TR_BigInt* _multiply_karatsuba(TR_BigInt* operand1, TR_BigInt* operand2)
{
	int maxSize,midpoint,expo;
	TR_BigInt *result,*tmp,*highOp1,*highOp2,*lowOp1,*lowOp2,*combined1,*combined2,*int1,*int2,*int3;
	char negative;
	negative = operand1->negative ^ operand2->negative;


	if (operand1->size < 2 || operand2->size < 2)
	{
		return _multiply_naive(operand1,operand2);
	}

	// Equalize operands
	if (operand2->size > operand1->size)
	{
		_pad(operand1,operand2->size);
	}
	else if (operand2->size < operand1->size)
	{
		_pad(operand2,operand1->size);
	}


	maxSize = operand1->size;
	midpoint = maxSize/2;

	/* It isn't all that important how we do this as long as we
	 * are consistent
	 */
	expo = maxSize%2==0?midpoint:midpoint+1;


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

	TR_BigInt_free(lowOp1);
	TR_BigInt_free(lowOp2);
	TR_BigInt_free(highOp1);
	TR_BigInt_free(highOp2);
	TR_BigInt_free(combined1);
	TR_BigInt_free(combined2);

	tmp = _karatsuba_safe_power(env,2*expo);
	result = _multiply_naive(int3,tmp);
	TR_BigInt_free(tmp);

	tmp = _karatsuba_safe_power(env,expo);
	combined1 = TR_BigInt_subtract(int2,int3);
	combined2 = TR_BigInt_subtract(combined1,int1);
	TR_BigInt_free(combined1);
	combined1 = _multiply_naive(combined2,tmp);

	TR_BigInt_free(tmp);

	tmp = TR_BigInt_add(result,combined1);
	TR_BigInt_free(result);
	result = tmp;

	tmp = TR_BigInt_add(result,int1);
	TR_BigInt_free(result);
	result = tmp;

	TR_BigInt_free(int1);
	TR_BigInt_free(int2);
	TR_BigInt_free(int3);
	TR_BigInt_free(combined1);
	TR_BigInt_free(combined2);

	result->negative = negative;

	_canonicalize(result);

	return result;
}

TR_BigInt* TR_BigInt_multiply(TR_BigInt* operand1, TR_BigInt* operand2)
{

	/* ~ 12 is when Karatsuba's exceeds performance of naive multiplication
	 * for radix of 10
	 */
	if (MAX(operand1->size, operand2->size) > 11)
	{
		return _multiply_karatsuba(operand1,operand2);
	}

	return _multiply_naive(operand1,operand2);
}

TR_BigInt_DivisionResult* TR_BigInt_divide(TR_BigInt* operand1, TR_BigInt* operand2)
{
	/* Naive division algorithm - very slow
	 * should rework it into a better integer division algorithm
	 */
	TR_BigInt *quotient, *remainder,*one,*zero,*tmp;
	TR_BigInt_DivisionResult* result;
	int diff;
	char negative;

	one = TR_BigInt_fromString(operand1->environment,"1");
	zero = TR_BigInt_fromString(operand1->environment,"0");
	quotient = zero;
	negative = operand1->negative ^ operand2->negative;
	operand1 = TR_BigInt_copy(operand1);
	operand2 = TR_BigInt_copy(operand2);
	operand1->negative = 0;
	operand2->negative = 0;
	remainder = operand1;
	diff = TR_BigInt_compare(remainder,operand2);
	while (diff == 0 || diff == 1)
	{
		tmp = TR_BigInt_add(quotient,one);
		TR_BigInt_free(quotient);
		quotient = tmp;

		tmp = TR_BigInt_subtract(remainder,operand2);
		TR_BigInt_free(remainder);
		remainder = tmp;

		diff = TR_BigInt_compare(remainder,operand2);

	}

	quotient->negative = negative;
	result = operand1->environment->allocator(sizeof(TR_BigInt_DivisionResult));
	_canonicalize(quotient);
	result->quotient = quotient;
	_canonicalize(remainder);
	result->remainder = remainder;

	TR_BigInt_free(one);
	return result;

}

TR_BigInt* TR_BigInt_gcd(TR_BigInt* op1, TR_BigInt* op2)
{
        TR_BigInt* num1,*num2;
	TR_BigInt_DivisionResult* divisionResult;
        TR_BigInt* zero = TR_BigInt_fromString(op1->environment,"0");

	/* Ensure correct ordering */
	if (TR_BigInt_compare(op1,op2) == 1)
	{
		return TR_BigInt_gcd(op2,op1);
	}
	num1 = TR_BigInt_absolute(op1);
	num2 = TR_BigInt_absolute(op2);
	while (TR_BigInt_compare(num1,zero) == 1)
	{
		divisionResult = TR_BigInt_divide(num2,num1);
		TR_BigInt_free(num2);
		num2 = num1;
		num1 = divisionResult->remainder;
		TR_BigInt_free(divisionResult->quotient);
		TR_BigInt_DivisionResult_free(divisionResult);
	}
	TR_BigInt_free(num1);
	TR_BigInt_free(zero);
	return num2;
}

/***************************/
/* DivisionResult Handling */
/***************************/
void TR_BigInt_DivisionResult_free(TR_BigInt_DivisionResult* toFree)
{
	toFree->environment->deallocator(toFree);
}

TR_BigInt* TR_BigInt_DivisionResult_remainder(TR_BigInt_DivisionResult* result)
{
	return result->remainder;
}

TR_BigInt* TR_BigInt_DivisionResult_quotient(TR_BigInt_DivisionResult* result)
{
  	return result->quotient;
}
