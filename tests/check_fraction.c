#include "Fraction.h"
#include <stdlib.h>
#include <check.h>

#define POSITIVE_CHECK_NUMBER "1/2"
#define NEGATIVE_CHECK_NUMBER "-1/2"
#define LARGE_CHECK_NUMBER "1982349823498234/128348923489234"

START_TEST (fraction_input_output)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Fraction* num1 = TR_Fraction_alloc(env);
	TR_Fraction* num2 = TR_Fraction_fromString(env,POSITIVE_CHECK_NUMBER);
	TR_Fraction* num3 = TR_Fraction_fromString(env,NEGATIVE_CHECK_NUMBER);
	TR_Fraction* num4 = TR_Fraction_fromString(env,LARGE_CHECK_NUMBER);
	
	ck_assert_str_eq(TR_Fraction_toString(num2),POSITIVE_CHECK_NUMBER);
	ck_assert_str_eq(TR_Fraction_toString(num3),NEGATIVE_CHECK_NUMBER);
	ck_assert_str_eq(TR_Fraction_toString(num4),LARGE_CHECK_NUMBER);
	
	TR_Fraction_free(num1);
	TR_Fraction_free(num2);
	TR_Fraction_free(num3);
	TR_Fraction_free(num4);
}
END_TEST


Suite* test_suite()
{
	Suite *suite;
	TCase *testInit;
	
	suite = suite_create("Fraction");
	testInit = tcase_create("Init");

	tcase_add_test(testInit,fraction_input_output);

	suite_add_tcase(suite,testInit);

	return suite;
}

