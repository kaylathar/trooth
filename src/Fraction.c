#include "Fraction.h"
#include "util.h"
#include "Environment_Internal.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct TR_Fraction
{
	TR_BigInt* numerator;
	TR_BigInt* denominator;
	TR_Environment* environment;
};

TR_Fraction* _canonicalize(TR_Fraction* operand);

TR_Fraction* TR_Fraction_alloc(TR_Environment* env)
{
	TR_Fraction* retVal =  env->allocator(sizeof(TR_Fraction));
	retVal->environment = env;
	return retVal;
}

TR_BigInt* TR_Fraction_numerator(TR_Fraction* operand) {
	return operand->numerator;
}
TR_BigInt* TR_Fraction_denominator(TR_Fraction* operand) {
	return operand->denominator;
}


TR_Fraction* TR_Fraction_copy(TR_Fraction *toCopy) {
	TR_Fraction* retVal = TR_Fraction_alloc(toCopy->environment);
	retVal->numerator = TR_BigInt_copy(toCopy->numerator);
	retVal->denominator = TR_BigInt_copy(toCopy->denominator);
	return retVal;
}

TR_Fraction* TR_Fraction_free(TR_Fraction *toFree) {
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

const char* TR_Fraction_toString(TR_Fraction *fraction) {
	return 0;
}
TR_Fraction* TR_Fraction_fromString(TR_Environment* env, const char* str) {
	return 0;
}

TR_Fraction* TR_Fraction_add(TR_Fraction* operand1, TR_Fraction* operand2) {
	return 0;
}
TR_Fraction* TR_Fraction_subtract(TR_Fraction* operand1, TR_Fraction* operand2) {
	return 0;
}
TR_Fraction* TR_Fraction_multiply(TR_Fraction* operand1, TR_Fraction* operand2) {
	TR_Fraction* result = TR_Fraction_alloc(operand1->environment);
	result->numerator = TR_BigInt_multiply(operand1->numerator,operand2->numerator);
	result->denominator = TR_BigInt_multiply(operand1->denominator,operand2->denominator);
	return _canonicalize(result);	
}
TR_Fraction* TR_Fraction_divide(TR_Fraction* operand1, TR_Fraction* operand2) {
	TR_Fraction* result = TR_Fraction_alloc(operand1->environment);
	result->numerator = TR_BigInt_multiply(operand1->numerator,operand2->denominator);
	result->denominator = TR_BigInt_multiply(operand1->denominator,operand2->numerator);
	return _canonicalize(result);
}

TR_Fraction* TR_Fraction_absolute(TR_Fraction* operand) {
	return 0;
}

TR_Fraction* _canonicalize(TR_Fraction* operand)
{
	return operand;
}

