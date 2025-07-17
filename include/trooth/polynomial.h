#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include "trooth/number.h"

struct TR_Polynomial;
typedef struct TR_Polynomial TR_Polynomial;

TR_Polynomial *TR_Polynomial_copy(TR_Polynomial *toCopy);
void TR_Polynomial_free(TR_Polynomial *toFree);
char *TR_Polynomial_toString(TR_Polynomial *Polynomial);
TR_Polynomial *TR_Polynomial_fromString(TR_Environment *env, const char *str);
TR_Polynomial *TR_Polynomial_fromNumbers(int minimumDegree, ...);

TR_Polynomial *TR_Polynomial_add(TR_Polynomial *operand1,
                                 TR_Polynomial *operand2);
TR_Polynomial *TR_Polynomial_multiply(TR_Polynomial *operand1,
                                      TR_Polynomial *operand2);
TR_Polynomial *TR_Polynomial_subtract(TR_Polynomial *operand1,
                                      TR_Polynomial *operand2);
TR_Polynomial *TR_Polynomial_divide(TR_Polynomial *operand1,
                                    TR_Polynomial *operand2);

int TR_Polynomial_compare(TR_Polynomial *operand1, TR_Polynomial *operand2);
char TR_Polynomial_equal(TR_Polynomial *operand1, TR_Polynomial *operand2);

#endif
