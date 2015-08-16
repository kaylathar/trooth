#include "trooth/rational.h"
#include "trooth/rational_internal.h"
#include "trooth/util_internal.h"
#include "trooth/environment_internal.h"
#include "trooth/bigint_internal.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

TR_Rational* _canonicalize(TR_Rational* operand);

TR_Rational* TR_Rational_alloc(TR_Environment* env)
{
	TR_Rational* retVal =  env->allocator(sizeof(TR_Rational));
	retVal->environment = env;
	retVal->numerator = NULL;
	retVal->denominator = NULL;
	return retVal;
}

TR_BigInt* TR_Rational_numerator(TR_Rational* operand) {
	return operand->numerator;
}
TR_BigInt* TR_Rational_denominator(TR_Rational* operand) {
	return operand->denominator;
}


TR_Rational* TR_Rational_copy(TR_Rational *toCopy) {
	TR_Rational* retVal = TR_Rational_alloc(toCopy->environment);
	retVal->numerator = TR_BigInt_copy(toCopy->numerator);
	retVal->denominator = TR_BigInt_copy(toCopy->denominator);
	return retVal;
}

void TR_Rational_free(TR_Rational *toFree) {
	if (toFree->numerator)
	{
		TR_BigInt_free(toFree->numerator);
	}

	if (toFree->denominator)
	{
		TR_BigInt_free(toFree->denominator);
	}
	toFree->environment->deallocator(toFree);
}

char* TR_Rational_toString(TR_Rational *rational) {
	char* numerator = TR_BigInt_toString(rational->numerator);
	char* denominator = TR_BigInt_toString(rational->denominator);
	int length = strlen(numerator)+strlen(denominator)+2;
	char* result = rational->environment->allocator(length);
	strcpy(result,numerator);
	strcat(result,"/");
	strcat(result,denominator);
	rational->environment->deallocator(numerator);
	rational->environment->deallocator(denominator);
	return result;
}

TR_Rational* TR_Rational_fromIntegers(TR_BigInt* numerator, TR_BigInt* denominator)
{
	TR_Rational* retVal = TR_Rational_alloc(numerator->environment);
	retVal->numerator = TR_BigInt_copy(numerator);
	retVal->denominator = TR_BigInt_copy(denominator);
	return _canonicalize(retVal);
}

TR_Rational* TR_Rational_fromString(TR_Environment* env, const char* str) {
	TR_Rational* retVal = TR_Rational_alloc(env);
	char* buffer = env->allocator(strlen(str)+1);
	strcpy(buffer,str);
	char* pos = strchr(buffer,'/');
	if (!pos) {
		return NULL;
	}
	*(pos) = '\0';

	retVal->numerator = TR_BigInt_fromString(env,buffer);
	retVal->denominator = TR_BigInt_fromString(env,pos+1);
	env->deallocator(buffer);

	return _canonicalize(retVal);
}

TR_Rational* TR_Rational_add(TR_Rational* operand1, TR_Rational* operand2) {
	/* Should work on cheaper way to compute LCM and use that instead */
	TR_BigInt* num1 = TR_BigInt_multiply(operand1->numerator,operand2->denominator);
	TR_BigInt* den = TR_BigInt_multiply(operand1->denominator,operand2->denominator);
	TR_BigInt* num2 = TR_BigInt_multiply(operand2->numerator,operand1->denominator);
	TR_Rational* retVal = TR_Rational_alloc(operand1->environment);
	retVal->numerator = TR_BigInt_add(num1,num2);
	retVal->denominator = den;
	TR_BigInt_free(num1);
	TR_BigInt_free(num2);
	return _canonicalize(retVal);
}
TR_Rational* TR_Rational_subtract(TR_Rational* operand1, TR_Rational* operand2) {
	/* Should work on cheaper way to compute LCM and use that instead */
        TR_BigInt* num1 = TR_BigInt_multiply(operand1->numerator,operand2->denominator);
        TR_BigInt* den = TR_BigInt_multiply(operand1->denominator,operand2->denominator);
        TR_BigInt* num2 = TR_BigInt_multiply(operand2->numerator,operand1->denominator);
        TR_Rational* retVal = TR_Rational_alloc(operand1->environment);
        retVal->numerator = TR_BigInt_subtract(num1,num2);
        retVal->denominator = den;
        TR_BigInt_free(num1);
        TR_BigInt_free(num2);
        return _canonicalize(retVal);
}
TR_Rational* TR_Rational_multiply(TR_Rational* operand1, TR_Rational* operand2) {
	TR_Rational* result = TR_Rational_alloc(operand1->environment);
	result->numerator = TR_BigInt_multiply(operand1->numerator,operand2->numerator);
	result->denominator = TR_BigInt_multiply(operand1->denominator,operand2->denominator);
	return _canonicalize(result);
}
TR_Rational* TR_Rational_divide(TR_Rational* operand1, TR_Rational* operand2) {
	TR_Rational* result = TR_Rational_alloc(operand1->environment);
	result->numerator = TR_BigInt_multiply(operand1->numerator,operand2->denominator);
	result->denominator = TR_BigInt_multiply(operand1->denominator,operand2->numerator);
	return _canonicalize(result);
}

TR_Rational* TR_Rational_absolute(TR_Rational* operand) {
	TR_Rational* retVal = TR_Rational_copy(operand);
	retVal->numerator = TR_BigInt_absolute(retVal->numerator);
	retVal->denominator = TR_BigInt_absolute(retVal->denominator);
	return retVal;
}

TR_Rational* _canonicalize(TR_Rational* operand)
{
	char negative = operand->numerator->negative ^ operand->denominator->negative;
	TR_BigInt* gcd = TR_BigInt_gcd(operand->numerator,operand->denominator);
	TR_BigInt_DivisionResult* num = TR_BigInt_divide(operand->numerator,gcd);
	TR_BigInt_DivisionResult* den = TR_BigInt_divide(operand->denominator,gcd);
	operand->numerator = TR_BigInt_copy(num->quotient);
	operand->numerator->negative = negative;
	operand->denominator = TR_BigInt_copy(den->quotient);
	operand->denominator->negative = 0;
	TR_BigInt_DivisionResult_free(den);
	TR_BigInt_DivisionResult_free(num);
	TR_BigInt_free(gcd);
	return operand;
}
