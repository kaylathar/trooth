#ifndef RATIONAL_H
#define RATIONAL_H
#include "trooth/bigint.h"

struct TR_Rational;
typedef struct TR_Rational TR_Rational;

TR_Rational* TR_Rational_copy(TR_Rational *toCopy);
void TR_Rational_free(TR_Rational *toFree);
char* TR_Rational_toString(TR_Rational *rational);
TR_Rational* TR_Rational_fromString(TR_Environment* env, const char* str);

TR_Rational* TR_Rational_add(TR_Rational* operand1, TR_Rational* operand2);
TR_Rational* TR_Rational_subtract(TR_Rational* operand1, TR_Rational* operand2);
TR_Rational* TR_Rational_multiply(TR_Rational* operand1, TR_Rational* operand2);
TR_Rational* TR_Rational_divide(TR_Rational* operand1, TR_Rational* operand2);
TR_Rational* TR_Rational_absolute(TR_Rational* operand);

int TR_Rational_compare(TR_Rational* operand1, TR_Rational* operand2);
char TR_Rational_greaterThan(TR_Rational *operand1, TR_Rational *operand2);
char TR_Rational_lessThan(TR_Rational* operand1, TR_Rational *operand2);
char TR_Rational_equal(TR_Rational* operand1, TR_Rational* operand2);

TR_BigInt* TR_Rational_numerator(TR_Rational* operand);
TR_BigInt* TR_Rational_denominator(TR_Rational* operand);

#endif
