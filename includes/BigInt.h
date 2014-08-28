#ifndef BIGINT_H
#define BIGINT_H
#include "Environment.h"
struct TR_BigInt;

typedef struct TR_BigInt TR_BigInt;

TR_BigInt* TR_BigInt_alloc(TR_Environment* env);
void TR_BigInt_free(TR_BigInt* number);
TR_BigInt* TR_BigInt_fromString(TR_Environment* env,const char* str);
const char* TR_BigInt_toString(TR_BigInt *number);
TR_BigInt* TR_BigInt_add(TR_BigInt *operand1, TR_BigInt *operand2);
TR_BigInt* TR_BigInt_subtract(TR_BigInt *operand1, TR_BigInt *operand2);
TR_BigInt* TR_BigInt_multiply(TR_BigInt *operand1, TR_BigInt *operand2);
TR_BigInt* TR_BigInt_copy(TR_BigInt *toCopy);
int TR_BigInt_compare(TR_BigInt *operand1, TR_BigInt *operand2);
char TR_BigInt_greaterThan(TR_BigInt *operand1, TR_BigInt *operand2);
char TR_BigInt_lessThan(TR_BigInt* operand1, TR_BigInt *operand2);
char TR_BigInt_equal(TR_BigInt* operand1, TR_BigInt* operand2);
#endif
