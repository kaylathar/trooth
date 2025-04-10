#include "trooth/rational.h"
#include <stdlib.h>
#include <check.h>
#include <stdio.h>

#define POSITIVE_CHECK_NUMBER "1/2"
#define POSITIVE_CHECK_NUMBER2 "1/4"
#define POSITIVE_CHECK_NUMBER3 "4/1"
#define NEGATIVE_CHECK_NUMBER "-1/2"
#define NEGATIVE_DOUBLE_CHECK_NUMBER "-1/-2"
#define NEGATIVE_NORMALIZE_CHECK_NUMBER "1/-2"

START_TEST (rational_input_output)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Rational* num2 = TR_Rational_fromString(env,POSITIVE_CHECK_NUMBER);
	TR_Rational* num3 = TR_Rational_fromString(env,NEGATIVE_CHECK_NUMBER);
	TR_Rational* num4 = TR_Rational_fromString(env,NEGATIVE_DOUBLE_CHECK_NUMBER);
	TR_Rational* num5 = TR_Rational_fromString(env,NEGATIVE_NORMALIZE_CHECK_NUMBER);

	ck_assert_str_eq(TR_Rational_toString(num2),POSITIVE_CHECK_NUMBER);
	ck_assert_str_eq(TR_Rational_toString(num3),NEGATIVE_CHECK_NUMBER);
	ck_assert_str_eq(TR_Rational_toString(num4),"1/2");
	ck_assert_str_eq(TR_Rational_toString(num5),"-1/2");

	TR_Rational_free(num2);
	TR_Rational_free(num3);
}
END_TEST

START_TEST (rational_copy)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Rational* num1 = TR_Rational_fromString(env,POSITIVE_CHECK_NUMBER);
	TR_Rational* num2 = TR_Rational_copy(num1);
	ck_assert_str_eq(TR_Rational_toString(num1),TR_Rational_toString(num2));
	ck_assert(num1 != num2);

}
END_TEST

START_TEST (rational_add)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Rational* num1 = TR_Rational_fromString(env,POSITIVE_CHECK_NUMBER);
        TR_Rational* num2 = TR_Rational_fromString(env,POSITIVE_CHECK_NUMBER2);
	TR_Rational* num3 = TR_Rational_add(num1,num2);
	ck_assert_str_eq(TR_Rational_toString(num3),"3/4");
}
END_TEST


START_TEST (rational_subtract)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Rational* num1 = TR_Rational_fromString(env,POSITIVE_CHECK_NUMBER);
        TR_Rational* num2 = TR_Rational_fromString(env,POSITIVE_CHECK_NUMBER2);
	TR_Rational* num3 = TR_Rational_subtract(num1,num2);
	ck_assert_str_eq(TR_Rational_toString(num3),"1/4");

}
END_TEST

START_TEST (rational_multiply)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Rational* num1 = TR_Rational_fromString(env,POSITIVE_CHECK_NUMBER);
        TR_Rational* num2 = TR_Rational_fromString(env,POSITIVE_CHECK_NUMBER2);
        TR_Rational* num3 = TR_Rational_multiply(num1,num2);
        ck_assert_str_eq(TR_Rational_toString(num3),"1/8");
}
END_TEST

START_TEST (rational_divide)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Rational* num1 = TR_Rational_fromString(env,POSITIVE_CHECK_NUMBER);
        TR_Rational* num2 = TR_Rational_fromString(env,POSITIVE_CHECK_NUMBER3);
        TR_Rational* num3 = TR_Rational_divide(num1,num2);
        ck_assert_str_eq(TR_Rational_toString(num3),"1/8");
}
END_TEST


Suite* test_suite()
{
	Suite *suite;
	TCase *testInit,*testOperations;

	suite = suite_create("Rational");
	testInit = tcase_create("Init");
	tcase_add_test(testInit,rational_input_output);
	tcase_add_test(testInit,rational_copy);

	testOperations = tcase_create("Operations");
	tcase_add_test(testOperations,rational_subtract);
	tcase_add_test(testOperations,rational_add);
	tcase_add_test(testOperations,rational_multiply);
	tcase_add_test(testOperations,rational_divide);

	suite_add_tcase(suite,testInit);
	suite_add_tcase(suite,testOperations);

	return suite;
}
