#include "trooth/number.h"
#include <stdlib.h>
#include <check.h>
#include <stdio.h>

#define NUM1 "1/2"
#define NUM2 "2"
#define NUM3 "-1/2"
#define NUM4 "-2"
#define NUM5 "1/4"

START_TEST (number_input_output)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Number* num1 = TR_Number_fromString(env,NUM1);
	TR_Number* num2 = TR_Number_fromString(env,NUM2);
	TR_Number* num3 = TR_Number_fromString(env,NUM3);
	TR_Number* num4 = TR_Number_fromString(env,NUM4);
	TR_Number* num5 = TR_Number_fromString(env,NUM5);

	ck_assert_str_eq(TR_Number_toString(num1),NUM1);
	ck_assert_str_eq(TR_Number_toString(num2),NUM2);
	ck_assert_str_eq(TR_Number_toString(num3),NUM3);
	ck_assert_str_eq(TR_Number_toString(num4),NUM4);
	ck_assert_str_eq(TR_Number_toString(num5),NUM5);

	TR_Number_free(num1);
	TR_Number_free(num2);
	TR_Number_free(num3);
	TR_Number_free(num4);
	TR_Number_free(num5);
}
END_TEST

START_TEST (number_copy)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Number* num1 = TR_Number_fromString(env,NUM1);
	TR_Number* num2 = TR_Number_copy(num1);
	ck_assert_str_eq(TR_Number_toString(num1),TR_Number_toString(num2));
	ck_assert(num1 != num2);

}
END_TEST

START_TEST (number_add)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Number* num1 = TR_Number_fromString(env,NUM1);
  TR_Number* num2 = TR_Number_fromString(env,NUM2);
	TR_Number* num3 = TR_Number_add(num1,num2);
	TR_Number* num4 = TR_Number_add(num1,num1);
	ck_assert_str_eq(TR_Number_toString(num3),"5/2");
	ck_assert_str_eq(TR_Number_toString(num4),"1");
}
END_TEST


START_TEST (number_subtract)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Number* num1 = TR_Number_fromString(env,NUM1);
  TR_Number* num2 = TR_Number_fromString(env,NUM5);
	TR_Number* num3 = TR_Number_subtract(num1,num2);
	ck_assert_str_eq(TR_Number_toString(num3),"1/4");

}
END_TEST

START_TEST (number_multiply)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Number* num1 = TR_Number_fromString(env,NUM1);
  TR_Number* num2 = TR_Number_fromString(env,NUM5);
  TR_Number* num3 = TR_Number_multiply(num1,num2);
  ck_assert_str_eq(TR_Number_toString(num3),"1/8");
}
END_TEST

START_TEST (number_divide)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_Number* num1 = TR_Number_fromString(env,NUM1);
  TR_Number* num2 = TR_Number_fromString(env,NUM2);
  TR_Number* num3 = TR_Number_divide(num1,num2);
  ck_assert_str_eq(TR_Number_toString(num3),"1/4");
}
END_TEST


Suite* test_suite()
{
	Suite *suite;
	TCase *testInit;

	suite = suite_create("Number");
	testInit = tcase_create("Init");


	tcase_add_test(testInit,number_input_output);
	tcase_add_test(testInit,number_subtract);
	tcase_add_test(testInit,number_add);
	tcase_add_test(testInit,number_multiply);
	tcase_add_test(testInit,number_divide);
	tcase_add_test(testInit,number_copy);

	suite_add_tcase(suite,testInit);

	return suite;
}
