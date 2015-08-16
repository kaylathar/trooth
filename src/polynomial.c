#include "trooth/polynomial.h"
#include "trooth/util_internal.h"
#include "trooth/environment_internal.h"
#include "trooth/bigint_internal.h"
#include "trooth/polynomial.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct TR_PolynomialTerm
{
  TR_Number* number;
  int degree; /* This means we do not support arbitrarily high degree polynomials, we can change this later */
};
typedef struct TR_PolynomialTerm TR_PolynomialTerm;

struct TR_Polynomial
{
  int numTerms;
	TR_Environment* environment;
  TR_PolynomialTerm* terms;
};

TR_Polynomial* TR_Polynomial_alloc(TR_Environment* env)
{
	TR_Polynomial* retVal =  env->allocator(sizeof(TR_Polynomial));
	retVal->environment = env;
	retVal->terms = NULL;
  retVal->numTerms = 0;
	return retVal;
}

TR_Polynomial* TR_Polynomial_copy(TR_Polynomial *toCopy) {
	TR_Polynomial* retVal = TR_Polynomial_alloc(toCopy->environment);
  retVal->terms = malloc(sizeof(TR_PolynomialTerm) * toCopy->numTerms);
  memcpy(retVal->terms,toCopy->terms,sizeof(TR_PolynomialTerm) * toCopy->numTerms);
  retVal->numTerms = toCopy->numTerms;
	return retVal;
}

void TR_Polynomial_free(TR_Polynomial *toFree) {
	if (toFree->terms)
	{
		toFree->environment->deallocator(toFree->terms);
	}
}

static void _canonicalize(TR_Polynomial *polynomial)
{

}

char* TR_Polynomial_toString(TR_Polynomial *polynomial) {
	return NULL;
}

TR_Polynomial* TR_Polynomial_fromString(TR_Environment* env, const char* str) {
	return NULL;
}

TR_Polynomial* TR_Polynomial_add(TR_Polynomial* operand1, TR_Polynomial* operand2) {
	return NULL;
}
TR_Polynomial* TR_Polynomial_subtract(TR_Polynomial* operand1, TR_Polynomial* operand2) {
  return NULL;
}
TR_Polynomial* TR_Polynomial_multiply(TR_Polynomial* operand1, TR_Polynomial* operand2) {
	return NULL;
}
TR_Polynomial* TR_Polynomial_divide(TR_Polynomial* operand1, TR_Polynomial* operand2) {
	return NULL;
}
