struct TR_BigInt {
  char *bytes;
  char negative;
  int size;
  TR_Environment *environment;
};

struct TR_BigInt_DivisionResult {
  TR_BigInt *quotient;
  TR_BigInt *remainder;
  TR_Environment *environment;
};
