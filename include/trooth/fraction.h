#ifndef FRACTION_H
#define FRACTION_H
#include "BigInt.h"

struct TR_Fraction;
typedef struct TR_Fraction TR_Fraction;

TR_Fraction* TR_Fraction_copy(TR_Fraction *toCopy);
void TR_Fraction_free(TR_Fraction *toFree);
char* TR_Fraction_toString(TR_Fraction *fraction);
TR_Fraction* TR_Fraction_fromString(TR_Environment* env, const char* str);

TR_Fraction* TR_Fraction_add(TR_Fraction* operand1, TR_Fraction* operand2);
TR_Fraction* TR_Fraction_subtract(TR_Fraction* operand1, TR_Fraction* operand2);
TR_Fraction* TR_Fraction_multiply(TR_Fraction* operand1, TR_Fraction* operand2);
TR_Fraction* TR_Fraction_divide(TR_Fraction* operand1, TR_Fraction* operand2);
TR_Fraction* TR_Fraction_absolute(TR_Fraction* operand);

int TR_Fraction_compare(TR_Fraction* operand1, TR_Fraction* operand2);
char TR_Fraction_greaterThan(TR_Fraction *operand1, TR_Fraction *operand2);
char TR_Fraction_lessThan(TR_Fraction* operand1, TR_Fraction *operand2);
char TR_Fraction_equal(TR_Fraction* operand1, TR_Fraction* operand2);

TR_BigInt* TR_Fraction_numerator(TR_Fraction* operand);
TR_BigInt* TR_Fraction_denominator(TR_Fraction* operand);

#endif
