#ifndef NUMBER_H
#define NUMBER_H
#include "trooth/bigint.h"
#include "trooth/rational.h"


enum TR_NumberType
{
  Fraction,
  BigInt
};

typedef enum TR_NumberType TR_NumberType;

struct TR_Number
{
  union {
    TR_Rational* rational;
    TR_BigInt* integer;
  } value;
  TR_NumberType type;
};
typedef struct TR_Number TR_Number;

#endif
