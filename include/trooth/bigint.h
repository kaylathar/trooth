#ifndef BIGINT_H
#define BIGINT_H
#include "trooth/environment.h"

/**
 * TR_BigInt represents a big-integer object, it is immutable
 *
 * The implementation of this object is opaque and no assumptions should be made
 */
struct TR_BigInt;
typedef struct TR_BigInt TR_BigInt;

/**
 * Represents a result of a division - made of of a quotient and remainder
 *
 * The implementation of this object is opaque and no assumptions should be made
 */
struct TR_BigInt_DivisionResult;
typedef struct TR_BigInt_DivisionResult TR_BigInt_DivisionResult;

/*************************/
/* Allocator/Deallocator */
/*************************/

/**
 * Allocate an empty BigInt - an empty BigInt is invalid for any operations
 * @param env Environment to allocate within - this will determine environment
 * all subsequent derived big ints are in - it is illegal to combine integer in
 * multiple environments in the same operation.
 * @return Allocated empty big integer
 */
TR_BigInt *TR_BigInt_alloc(TR_Environment *env);

/**
 * Frees memory used by a BigInt object - <b>you are responsible for freeing any
 * objects that result from your calculations unless the API specifies
 * otherwise</b>
 */
void TR_BigInt_free(TR_BigInt *number);

/**
 * Copy toCopy and return a new big integer with identical value
 * @return Copied big integer object
 */
TR_BigInt *TR_BigInt_copy(TR_BigInt *toCopy);

/**
 * Frees a given division result - will also free all objects it owns <b>note
 * that freeing internal opaque objects within a division result is invalid</b>
 */
void TR_BigInt_DivisionResult_free(TR_BigInt_DivisionResult *toFree);

/*******************/
/* Convert to/from */
/*******************/

/**
 * Convert a c-string representation of a radix-10 number into a big integer
 * @param env The environment to place this big int in - this environment will
 * be used for any calculations this big int is used in as well - using integers
 * from multiple environments in the same operation is not permitted.
 * @param str Null-terminated c-string in radix-10 format
 */
TR_BigInt *TR_BigInt_fromString(TR_Environment *env, const char *str);

/**
 * Convert a big integer object into a null terminated C-string
 * @return Null terminated c-string representation of given big integer in
 * radix-10
 */
char *TR_BigInt_toString(TR_BigInt *number);

/*************************/
/* Arithmetic operations */
/*************************/

/**
 * Adds operand1 to operand2 and returns the result
 * @return Result of addition of operand1 and operand2
 * */
TR_BigInt *TR_BigInt_add(TR_BigInt *operand1, TR_BigInt *operand2);

/**
 * Subracts operand2 from operand2 and returns the result
 * @return Result of subtraction of operand1 by operand2
 * */
TR_BigInt *TR_BigInt_subtract(TR_BigInt *operand1, TR_BigInt *operand2);

/**
 * Multiplies operand1 and operand2 and returns the result
 * @return Result of multiplication of operand1 and operand2
 * */
TR_BigInt *TR_BigInt_multiply(TR_BigInt *operand1, TR_BigInt *operand2);

/**
 * Exponentiate a big int by another big int
 * Only supports positive exponents - undefined results for negative
 * @return Result of operand1 raised to exponent operand2
 */
TR_BigInt *TR_BigInt_exponentiate(TR_BigInt *operand1, TR_BigInt *operand2);

/**
 * Divides operand1 by operand2 and returns the result
 * @return Result of dividing operand1 by operand2
 * */
TR_BigInt_DivisionResult *TR_BigInt_divide(TR_BigInt *operand1,
                                           TR_BigInt *operand2);

/**
 * Takes absolute value of operand
 * @return Absolute value of operand
 * */
TR_BigInt *TR_BigInt_absolute(TR_BigInt *operand);

/**
 * Calculates GCD of two numbers, please make no assumptions about algorithm
 * being used as it is subject to change
 * @return GCD of the two values given - if no common divisors but 1 exist,
 * function will return 1
 */
TR_BigInt *TR_BigInt_gcd(TR_BigInt *op1, TR_BigInt *op2);

/***************/
/* Comparisons */
/***************/

/**
 * Performs comparison of operand1 and operand2
 * @param operand1 First big integer to compare
 * @param operand2 Second big integer to compare
 * @return -1 if operand1 < operand2, 0 if operand1 == operand2, 1 if opreand1 >
 * operand2
 */
int TR_BigInt_compare(TR_BigInt *operand1, TR_BigInt *operand2);

/**
 * Checks if operand1 is greater than operand2
 * @return 1 if operand1 > operand2
 */
char TR_BigInt_greaterThan(TR_BigInt *operand1, TR_BigInt *operand2);

/**
 * Checks if operand1 is less than operand2
 * @return 1 if operand1 < operand2
 */
char TR_BigInt_lessThan(TR_BigInt *operand1, TR_BigInt *operand2);

/**
 * Checks if operand1 is equal to operand2
 * @return 1 if operand1 = operand2
 */
char TR_BigInt_equal(TR_BigInt *operand1, TR_BigInt *operand2);

/*******************************/
/* Division Result Information */
/*******************************/

/**
 * Returns remainder of a division result - <b>the returned remainder remains
 * owned by the result object, do not free it</b>
 */
TR_BigInt *TR_BigInt_DivisionResult_remainder(TR_BigInt_DivisionResult *result);

/**
 * Returns quotient of a division result - <b>the returned remainder remains
 * owned by the result object, do not free it</b>
 */
TR_BigInt *TR_BigInt_DivisionResult_quotient(TR_BigInt_DivisionResult *result);

#endif
