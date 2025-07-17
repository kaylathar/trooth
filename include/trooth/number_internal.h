#ifndef NUMBER_INTERNAL_H
#define NUMBER_INTERNAL_H
#include "trooth/bigint.h"
#include "trooth/number.h"
#include "trooth/rational.h"

enum TR_NumberType { Empty, Rational, BigInt };

typedef enum TR_NumberType TR_NumberType;

struct TR_Number {
  TR_Environment *environment;
  union {
    TR_Rational *rational;
    TR_BigInt *integer;
  } value;
  TR_NumberType type;
};

#endif
