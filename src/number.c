#include "trooth/number_internal.h"
#include "trooth/environment_internal.h"
#include "trooth/rational_internal.h"
#include "trooth/bigint_internal.h"
#include <stdlib.h>
#include <string.h>

static TR_Number* TR_Number_alloc(TR_Environment* environment)
{
  TR_Number* retVal;

  retVal = environment->allocator(sizeof(TR_Number));
  memset(retVal,0,sizeof(TR_Number));
  retVal->environment = environment;
  retVal->type = Empty;

  return retVal;
}

static void _upconvert(TR_Number* value)
{
  TR_BigInt *one;
  TR_BigInt *old_integer;

  if (value->type == Rational)
  {
    return;
  }

  one = TR_BigInt_fromString(value->environment,"1");
  old_integer = value->value.integer;
  value->type = Rational;
  value->value.rational = TR_Rational_fromIntegers(old_integer,one);
  TR_BigInt_free(old_integer);
  TR_BigInt_free(one);
}

static void _upconvert_pair(TR_Number* value1, TR_Number* value2)
{
  _upconvert(value1);
  _upconvert(value2);
}

static void _downconvert(TR_Number* value)
{
  TR_BigInt* denominator;
  TR_Rational* old_rational;
  if (value->type == BigInt)
  {
    return;
  }

  denominator = TR_Rational_denominator(value->value.rational);

  /* When we have a one constant, switch to that */
  if (strcmp(TR_BigInt_toString(denominator),"1") != 0)
  {
    return;
  }

  old_rational = value->value.rational;
  value->value.integer = TR_BigInt_copy(TR_Rational_numerator(old_rational));
  TR_Rational_free(old_rational);
  value->type = BigInt;
}

static TR_Number* _performArithmetic(TR_Rational* (*rationalFunc)(TR_Rational*,TR_Rational*), TR_BigInt* (*bigintFunc)(TR_BigInt*,TR_BigInt*),TR_Number* operand1, TR_Number* operand2)
{
  TR_Number *op1, *op2, *result;
  result = TR_Number_alloc(operand1->environment);

  if (operand1->type == Rational || operand2->type == Rational)
  {
    op1 = TR_Number_copy(operand1);
    op2 = TR_Number_copy(operand2);
    _upconvert_pair(op1,op2);
    result->type = Rational;
    result->value.rational = rationalFunc(op1->value.rational,op2->value.rational);
    _downconvert(result);
    TR_Number_free(op1);
    TR_Number_free(op2);
    return result;
  }

  result->type = BigInt;
  result->value.integer = bigintFunc(operand1->value.integer,operand2->value.integer);
  return result;
}


char* TR_Number_toString(TR_Number* number)
{
  if (number->type == BigInt)
  {
    return TR_BigInt_toString(number->value.integer);
  } else if (number->type == Rational) {
    return TR_Rational_toString(number->value.rational);
  }

  return NULL;
}

TR_Number* TR_Number_fromString(TR_Environment* env, const char* input)
{
  TR_Number* number = TR_Number_alloc(env);
  if (strchr(input,'/'))
  {
      number->value.rational = TR_Rational_fromString(env, input);
      number->type = Rational;
      _downconvert(number);
      return number;
  }

  number->type = BigInt;
  number->value.integer = TR_BigInt_fromString(env,input);
  return number;
}

TR_Number* TR_Number_fromRational(TR_Rational *toWrap)
{
  TR_Number *result = TR_Number_alloc(toWrap->environment);
  result->value.rational = TR_Rational_copy(toWrap);
  result->type = Rational;
  return result;
}

TR_Number* TR_Number_fromBigInt(TR_BigInt *toWrap)
{
  TR_Number *result = TR_Number_alloc(toWrap->environment);
  result->value.integer = TR_BigInt_copy(toWrap);
  result->type = BigInt;
  return result;
}

TR_Number* TR_Number_copy(TR_Number *toCopy)
{
  TR_Number *result = TR_Number_alloc(toCopy->environment);
  if (toCopy->type == Rational)
  {
    result->type = Rational;
    result->value.rational = TR_Rational_copy(toCopy->value.rational);
  } else if (toCopy->type == BigInt) {
    result->type = BigInt;
    result->value.integer = TR_BigInt_copy(toCopy->value.integer);
  }
  return result;
}

void TR_Number_free(TR_Number *toFree)
{
  if (toFree->type == Rational)
  {
    TR_Rational_free(toFree->value.rational);
  } else if (toFree->type == BigInt) {
    TR_BigInt_free(toFree->value.integer);
  }
  toFree->environment->deallocator(toFree);
}

TR_Number* TR_Number_add(TR_Number* operand1, TR_Number* operand2)
{
  return _performArithmetic(TR_Rational_add,TR_BigInt_add, operand1, operand2);
}

TR_Number* TR_Number_multiply(TR_Number* operand1, TR_Number* operand2)
{
  return _performArithmetic(TR_Rational_multiply,TR_BigInt_multiply, operand1, operand2);
}

TR_Number* TR_Number_subtract(TR_Number* operand1, TR_Number* operand2)
{
  return _performArithmetic(TR_Rational_subtract,TR_BigInt_subtract, operand1, operand2);
}

TR_Number* TR_Number_divide(TR_Number* operand1, TR_Number* operand2)
{
  /* You say potato, I say tomato... */
  TR_Number *op1, *op2, *result;
  
  op1 = TR_Number_copy(operand1);
  op2 = TR_Number_copy(operand2);
  _upconvert_pair(op1, op2);
  
  result = TR_Number_alloc(operand1->environment);
  result->type = Rational;
  result->value.rational = TR_Rational_divide(op1->value.rational, op2->value.rational);
  _downconvert(result);
  
  TR_Number_free(op1);
  TR_Number_free(op2);
  return result;
}

TR_Number* TR_Number_absolute(TR_Number* operand)
{
  TR_Number* retVal = TR_Number_alloc(operand->environment);
  if (operand->type == Rational)
  {
    retVal->value.rational = TR_Rational_absolute(operand->value.rational);
    retVal->type = Rational;
  }

  else
  {
    retVal->value.integer = TR_BigInt_absolute(operand->value.integer);
    retVal->type = BigInt;
  }

  return retVal;
}


int TR_Number_compare(TR_Number* operand1, TR_Number* operand2)
{
  TR_Number *op1, *op2;

  if (operand1->type == Rational || operand2->type == Rational)
  {
    op1 = TR_Number_copy(operand1);
    op2 = TR_Number_copy(operand2);
    _upconvert_pair(op1,op2);
    return TR_Rational_compare(op1->value.rational,op2->value.rational);
  }

  return TR_BigInt_compare(op1->value.integer,op2->value.integer);
}

char TR_Number_greaterThan(TR_Number *operand1, TR_Number *operand2)
{
  return TR_Number_compare(operand1,operand2) > 0;
}

char TR_Number_lessThan(TR_Number* operand1, TR_Number *operand2)
{
  return TR_Number_compare(operand1, operand2) < 0;
}

char TR_Number_equal(TR_Number* operand1, TR_Number* operand2)
{
  return TR_Number_compare(operand1,operand2) == 0;
}
