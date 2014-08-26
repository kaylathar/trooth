#include "Environment.h"
struct TR_BigInt;

typedef struct TR_BigInt TR_BigInt;

TR_BigInt* TR_BigInt_alloc(TR_Environment* env);
void TR_BigInt_free(TR_Environment* env, TR_BigInt* number);
TR_BigInt* TR_BigInt_fromString(TR_Environment* env,const char* str);
const char* TR_BigInt_toString(TR_Environment* env,TR_BigInt *number);
TR_BigInt* TR_BigInt_add(TR_Environment* env,TR_BigInt *operand1, TR_BigInt *operand2);
TR_BigInt* TR_BigInt_subtract(TR_Environment* env,TR_BigInt *operand1, TR_BigInt *operand2);
TR_BigInt* TR_BigInt_copy(TR_Environment*,TR_BigInt *toCopy);
char TR_BigInt_compare(TR_Environment*,TR_BigInt *operand1, TR_BigInt *operand2);
