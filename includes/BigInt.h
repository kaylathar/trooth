struct TR_BigInt;

typedef struct TR_BigInt TR_BigInt;

TR_BigInt* TR_BigInt_alloc();
void TR_BigInt_free(TR_BigInt* number);
TR_BigInt* TR_BigInt_fromString(const char* str);
const char* TR_BigInt_toString(TR_BigInt *number);
TR_BigInt* TR_BigInt_add(TR_BigInt *operand1, TR_BigInt *operand2);
TR_BigInt* TR_BigInt_subtract(TR_BigInt *operand1, TR_BigInt *operand2);
TR_BigInt* TR_BigInt_copy(TR_BigInt *toCopy);
