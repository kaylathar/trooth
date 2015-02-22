#include "Fraction.h"
#include <stdlib.h>
#include <check.h>
#include <stdio.h>

#define POSITIVE_CHECK_NUMBER "1/2"
#define POSITIVE_CHECK_NUMBER2 "1/4"
#define POSITIVE_CHECK_NUMBER3 "4/1"
#define NEGATIVE_CHECK_NUMBER "-1/2"

START_TEST (fraction_input_output)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Fraction* num2 = TR_Fraction_fromString(env,POSITIVE_CHECK_NUMBER);
	TR_Fraction* num3 = TR_Fraction_fromString(env,NEGATIVE_CHECK_NUMBER);
	
	ck_assert_str_eq(TR_Fraction_toString(num2),POSITIVE_CHECK_NUMBER);
	ck_assert_str_eq(TR_Fraction_toString(num3),NEGATIVE_CHECK_NUMBER);
	
	TR_Fraction_free(num2);
	TR_Fraction_free(num3);
}
END_TEST

START_TEST (fraction_copy)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Fraction* num1 = TR_Fraction_fromString(env,POSITIVE_CHECK_NUMBER);
	TR_Fraction* num2 = TR_Fraction_copy(num1);
	ck_assert_str_eq(TR_Fraction_toString(num1),TR_Fraction_toString(num2));
	ck_assert(num1 != num2);

}
END_TEST

START_TEST (fraction_add)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Fraction* num1 = TR_Fraction_fromString(env,POSITIVE_CHECK_NUMBER);
        TR_Fraction* num2 = TR_Fraction_fromString(env,POSITIVE_CHECK_NUMBER2);
	TR_Fraction* num3 = TR_Fraction_add(num1,num2);
	ck_assert_str_eq(TR_Fraction_toString(num3),"3/4");
}
END_TEST


START_TEST (fraction_subtract)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Fraction* num1 = TR_Fraction_fromString(env,POSITIVE_CHECK_NUMBER);
        TR_Fraction* num2 = TR_Fraction_fromString(env,POSITIVE_CHECK_NUMBER2);
	fprintf(stderr,"Checking it out...\n");
	TR_Fraction* num3 = TR_Fraction_subtract(num1,num2);
	ck_assert_str_eq(TR_Fraction_toString(num3),"1/4");

}
END_TEST

START_TEST (fraction_multiply)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Fraction* num1 = TR_Fraction_fromString(env,POSITIVE_CHECK_NUMBER);
        TR_Fraction* num2 = TR_Fraction_fromString(env,POSITIVE_CHECK_NUMBER2);
        TR_Fraction* num3 = TR_Fraction_multiply(num1,num2);
        ck_assert_str_eq(TR_Fraction_toString(num3),"1/8");
}
END_TEST

START_TEST (fraction_divide)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Fraction* num1 = TR_Fraction_fromString(env,POSITIVE_CHECK_NUMBER);
        TR_Fraction* num2 = TR_Fraction_fromString(env,POSITIVE_CHECK_NUMBER3);
        TR_Fraction* num3 = TR_Fraction_divide(num1,num2);
        ck_assert_str_eq(TR_Fraction_toString(num3),"1/8");
}
END_TEST


Suite* test_suite()
{
	Suite *suite;
	TCase *testInit;
	
	suite = suite_create("Fraction");
	testInit = tcase_create("Init");


	tcase_add_test(testInit,fraction_input_output);
	tcase_add_test(testInit,fraction_subtract);
	tcase_add_test(testInit,fraction_add);
	tcase_add_test(testInit,fraction_multiply);
	tcase_add_test(testInit,fraction_divide);
	tcase_add_test(testInit,fraction_copy);
	
	suite_add_tcase(suite,testInit);

	return suite;
}

