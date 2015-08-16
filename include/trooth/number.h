#ifndef NUMBER_H
#define NUMBER_H
#include <trooth/rational.h>
#include <trooth/bigint.h>
#include <trooth/environment.h>

struct TR_Number;
typedef struct TR_Number TR_Number;

TR_Number* TR_Number_fromRational(TR_Rational *toWrap);
TR_Number* TR_Number_fromBigInt(TR_BigInt *toWrap);
TR_Number* TR_Number_copy(TR_Number *toCopy);
void TR_Number_free(TR_Number *toFree);

TR_Number* TR_Number_add(TR_Number* operand1, TR_Number* operand2);
TR_Number* TR_Number_multiply(TR_Number* operand1, TR_Number* operand2);
TR_Number* TR_Number_subtract(TR_Number* operand1, TR_Number* operand2);
TR_Number* TR_Number_divide(TR_Number* operand1, TR_Number* operand2);
TR_Number* TR_Number_absolute(TR_Number* operand);

int TR_Number_compare(TR_Number* operand1, TR_Number* operand2);
char TR_Number_greaterThan(TR_Number *operand1, TR_Number *operand2);
char TR_Number_lessThan(TR_Number* operand1, TR_Number *operand2);
char TR_Number_equal(TR_Number* operand1, TR_Number* operand2);

#endif
