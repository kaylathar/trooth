#include "trooth/bigint.h"
#include <stdlib.h>
#include <check.h>

#define LARGE_CHECK_NUMBER "222222222222222222222222222222222222"
#define LARGE_CHECK_NUMBER2 "384756384756473847564837463847563847"
#define LARGE_CHECK_NUMBER_TIMES_LARGE_CHECK_NUMBER2 "85501418834771966125519436410569743692276358943005811652258341367208034"
#define POSITIVE_CHECK_NUMBER "1234"
#define NEGATIVE_CHECK_NUMBER "-1234"

START_TEST (bigint_init)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_BigInt* num1 = TR_BigInt_alloc(env);
	TR_BigInt* num2 = TR_BigInt_fromString(env,POSITIVE_CHECK_NUMBER);
	TR_BigInt* num3 = TR_BigInt_fromString(env,NEGATIVE_CHECK_NUMBER);
	TR_BigInt* num4 = TR_BigInt_fromString(env,LARGE_CHECK_NUMBER);
	
	ck_assert_str_eq(TR_BigInt_toString(num2),POSITIVE_CHECK_NUMBER);
	ck_assert_str_eq(TR_BigInt_toString(num3),NEGATIVE_CHECK_NUMBER);
	ck_assert_str_eq(TR_BigInt_toString(num4),LARGE_CHECK_NUMBER);
	
	TR_BigInt_free(num1);
	TR_BigInt_free(num2);
	TR_BigInt_free(num3);
	TR_BigInt_free(num4);
}
END_TEST

START_TEST (bigint_gcd)
{
	TR_Environment* env = TR_Environment_alloc();
        TR_BigInt* num1 = TR_BigInt_fromString(env,"12");
        TR_BigInt* num2 = TR_BigInt_fromString(env,"4");
        TR_BigInt* num3 = TR_BigInt_fromString(env,"3");
        TR_BigInt* num4 = TR_BigInt_fromString(env,"8");

	TR_BigInt* result1 = TR_BigInt_gcd(num1,num1);
	TR_BigInt* result2 = TR_BigInt_gcd(num1,num2);
	TR_BigInt* result3 = TR_BigInt_gcd(num4,num1);
	TR_BigInt* result4 = TR_BigInt_gcd(num2,num3);

	ck_assert_str_eq(TR_BigInt_toString(result1),"12");
	ck_assert_str_eq(TR_BigInt_toString(result2),"4");
	ck_assert_str_eq(TR_BigInt_toString(result3),"4");
	ck_assert_str_eq(TR_BigInt_toString(result4),"1");
	
}
END_TEST

START_TEST (bigint_add)
{
	TR_Environment* env = TR_Environment_alloc();
	TR_BigInt* num1 = TR_BigInt_fromString(env,"1234");
	TR_BigInt* num2 = TR_BigInt_fromString(env,"-1234");
	TR_BigInt* num3 = TR_BigInt_fromString(env,"12");
	TR_BigInt* num4 = TR_BigInt_fromString(env,"-12");

	// Test positive/positive
	TR_BigInt* result1 = TR_BigInt_add(num1,num3);
	TR_BigInt* result2 = TR_BigInt_add(num3,num1);
	ck_assert_str_eq(TR_BigInt_toString(result1),"1246");
	ck_assert_str_eq(TR_BigInt_toString(result2),"1246");

	// Test negative/negative
	TR_BigInt* result3 = TR_BigInt_add(num2,num4);
	TR_BigInt* result4 = TR_BigInt_add(num4,num2);
	ck_assert_str_eq(TR_BigInt_toString(result3),"-1246");
	ck_assert_str_eq(TR_BigInt_toString(result4),"-1246");

	// Test negative/positive
	TR_BigInt* result5 = TR_BigInt_add(num1,num4);
	TR_BigInt* result6 = TR_BigInt_add(num4,num1);
	ck_assert_str_eq(TR_BigInt_toString(result5),"1222");
	ck_assert_str_eq(TR_BigInt_toString(result6),"1222");

	TR_BigInt_free(num1);
	TR_BigInt_free(num2);
	TR_BigInt_free(num3);
	TR_BigInt_free(num4);
        TR_BigInt_free(result1);
        TR_BigInt_free(result2);
        TR_BigInt_free(result3);
        TR_BigInt_free(result4);
        TR_BigInt_free(result5);
        TR_BigInt_free(result6);
	TR_Environment_free(env);
}
END_TEST

START_TEST (bigint_subtract)
{
	TR_Environment* env = TR_Environment_alloc();
        TR_BigInt* num1 = TR_BigInt_fromString(env,"1234");
        TR_BigInt* num2 = TR_BigInt_fromString(env,"-1234");
        TR_BigInt* num3 = TR_BigInt_fromString(env,"12");
        TR_BigInt* num4 = TR_BigInt_fromString(env,"-12");

        // Test positive/positive
        TR_BigInt* result1 = TR_BigInt_subtract(num1,num3);
        TR_BigInt* result2 = TR_BigInt_subtract(num3,num1);
        ck_assert_str_eq(TR_BigInt_toString(result1),"1222");
        ck_assert_str_eq(TR_BigInt_toString(result2),"-1222");

        // Test negative/negative
        TR_BigInt* result3 = TR_BigInt_subtract(num2,num4);
        TR_BigInt* result4 = TR_BigInt_subtract(num4,num2);
        ck_assert_str_eq(TR_BigInt_toString(result3),"-1222");
        ck_assert_str_eq(TR_BigInt_toString(result4),"1222");

        TR_BigInt_free(num1);
        TR_BigInt_free(num2);
	TR_BigInt_free(num3);
	TR_BigInt_free(num4);
	TR_BigInt_free(result1);
	TR_BigInt_free(result2);
	TR_BigInt_free(result3);
	TR_BigInt_free(result4);
	TR_Environment_free(env);

}
END_TEST

START_TEST (bigint_compare)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_BigInt *num1 = TR_BigInt_fromString(env,"-12");
	TR_BigInt *num2 = TR_BigInt_fromString(env,"12");
	TR_BigInt *num3 = TR_BigInt_fromString(env,"-1234");
	TR_BigInt *num4 = TR_BigInt_fromString(env,"1234");
	TR_BigInt *num5 = TR_BigInt_fromString(env,"1234");

	// Test positive/positive
	ck_assert_int_eq(TR_BigInt_compare(num2,num4),-1);
	ck_assert_int_eq(TR_BigInt_compare(num4,num2),1);
	
	// Test equality and idempotency
	ck_assert_int_eq(TR_BigInt_compare(num4,num4),0);
	ck_assert_int_eq(TR_BigInt_compare(num4,num5),0);
	ck_assert_int_eq(TR_BigInt_compare(num5,num4),0);

	// Verify convenience functions
	ck_assert(TR_BigInt_greaterThan(num4,num2)==1);
	ck_assert(TR_BigInt_greaterThan(num2,num4)==0);
	ck_assert(TR_BigInt_greaterThan(num1,num3)==1);
	ck_assert(TR_BigInt_greaterThan(num3,num1)==0);
	ck_assert(TR_BigInt_greaterThan(num4,num5)==0);
	ck_assert(TR_BigInt_lessThan(num4,num2)==0);
	ck_assert(TR_BigInt_lessThan(num2,num4)==1);
	ck_assert(TR_BigInt_lessThan(num1,num3)==0);
	ck_assert(TR_BigInt_lessThan(num3,num1)==1);
	ck_assert(TR_BigInt_lessThan(num4,num5)==0);
	ck_assert(TR_BigInt_equal(num4,num5)==1);
	ck_assert(TR_BigInt_equal(num2,num3)==0);

	// Test negative/negative
	ck_assert_int_eq(TR_BigInt_compare(num1,num3),1);
	ck_assert_int_eq(TR_BigInt_compare(num3,num1),-1);

	// Test negative/positive
	ck_assert_int_eq(TR_BigInt_compare(num3,num4),-1);
	ck_assert_int_eq(TR_BigInt_compare(num4,num3),1);

	// Clean-up
	TR_BigInt_free(num1);
	TR_BigInt_free(num2);
	TR_BigInt_free(num3);
	TR_BigInt_free(num4);
	TR_BigInt_free(num5);
	TR_Environment_free(env);
}
END_TEST

START_TEST(bigint_multiply)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_BigInt *num1 = TR_BigInt_fromString(env,"123");
	TR_BigInt *num2 = TR_BigInt_fromString(env,"-123");
	TR_BigInt *num3 = TR_BigInt_fromString(env,"12");
	TR_BigInt *num4 = TR_BigInt_fromString(env,"-12");
	TR_BigInt *num5 = TR_BigInt_fromString(env,LARGE_CHECK_NUMBER);
	TR_BigInt *num6 = TR_BigInt_fromString(env,LARGE_CHECK_NUMBER2);

	// pos/pos
	TR_BigInt *result1 = TR_BigInt_multiply(num3,num1);
	TR_BigInt *result2 = TR_BigInt_multiply(num1,num3);
	ck_assert(TR_BigInt_equal(result1,result2));
	ck_assert_str_eq(TR_BigInt_toString(result1),"1476");
	

	// neg/neg
	TR_BigInt *result3 = TR_BigInt_multiply(num2,num4);
	TR_BigInt *result4 = TR_BigInt_multiply(num4,num2);
	ck_assert(TR_BigInt_equal(result3,result4));
	ck_assert_str_eq(TR_BigInt_toString(result3),"1476");

	// pos/neg
	TR_BigInt *result5 = TR_BigInt_multiply(num1,num4);	
	TR_BigInt *result6 = TR_BigInt_multiply(num4,num1);
	ck_assert(TR_BigInt_equal(result5,result6));
	ck_assert_str_eq(TR_BigInt_toString(result5),"-1476");

	// large
	TR_BigInt *result7 = TR_BigInt_multiply(num5,num6);
	ck_assert_str_eq(TR_BigInt_toString(result7),LARGE_CHECK_NUMBER_TIMES_LARGE_CHECK_NUMBER2);


	TR_BigInt_free(result1);
	TR_BigInt_free(result2);
	TR_BigInt_free(result3);
	TR_BigInt_free(result4);
	TR_BigInt_free(result5);
	TR_BigInt_free(result6);
	TR_BigInt_free(result7);
	TR_BigInt_free(num1);
	TR_BigInt_free(num2);
	TR_BigInt_free(num3);
	TR_BigInt_free(num4);
	TR_BigInt_free(num5);
	TR_BigInt_free(num6);
	TR_Environment_free(env);
}
END_TEST


START_TEST(bigint_divide)
{
	TR_Environment *env = TR_Environment_alloc();
	TR_BigInt *num1 = TR_BigInt_fromString(env,"1200");
	TR_BigInt *num2 = TR_BigInt_fromString(env,"-1200");
	TR_BigInt *num3 = TR_BigInt_fromString(env,"12");
	TR_BigInt *num4 = TR_BigInt_fromString(env,"-12");
	TR_BigInt *num5 = TR_BigInt_fromString(env,LARGE_CHECK_NUMBER);
	TR_BigInt *num6 = TR_BigInt_fromString(env,LARGE_CHECK_NUMBER2);

	// pos/pos
	TR_BigInt_DivisionResult *result1 = TR_BigInt_divide(num3,num1);
	TR_BigInt_DivisionResult *result2 = TR_BigInt_divide(num1,num3);
	ck_assert_str_eq(TR_BigInt_toString(TR_BigInt_DivisionResult_quotient(result1)),"0");
	ck_assert_str_eq(TR_BigInt_toString(TR_BigInt_DivisionResult_remainder(result1)),"12");
	ck_assert_str_eq(TR_BigInt_toString(TR_BigInt_DivisionResult_quotient(result2)),"100");
	ck_assert_str_eq(TR_BigInt_toString(TR_BigInt_DivisionResult_remainder(result2)),"0");

	// neg/neg
	TR_BigInt_DivisionResult *result3 = TR_BigInt_divide(num2,num4);
	TR_BigInt_DivisionResult *result4 = TR_BigInt_divide(num4,num2);
	ck_assert_str_eq(TR_BigInt_toString(TR_BigInt_DivisionResult_quotient(result3)),"100");
	ck_assert_str_eq(TR_BigInt_toString(TR_BigInt_DivisionResult_remainder(result3)),"0");
	ck_assert_str_eq(TR_BigInt_toString(TR_BigInt_DivisionResult_quotient(result4)),"0");
	ck_assert_str_eq(TR_BigInt_toString(TR_BigInt_DivisionResult_remainder(result4)),"12");

	// pos/neg
	TR_BigInt_DivisionResult *result5 = TR_BigInt_divide(num1,num4);	
	TR_BigInt_DivisionResult *result6 = TR_BigInt_divide(num4,num1);
	ck_assert_str_eq(TR_BigInt_toString(TR_BigInt_DivisionResult_quotient(result5)),"-100");
	ck_assert_str_eq(TR_BigInt_toString(TR_BigInt_DivisionResult_remainder(result5)),"0");
	ck_assert_str_eq(TR_BigInt_toString(TR_BigInt_DivisionResult_quotient(result6)),"0");
	ck_assert_str_eq(TR_BigInt_toString(TR_BigInt_DivisionResult_remainder(result6)),"12");

	TR_BigInt_free(num1);
	TR_BigInt_free(num2);
	TR_BigInt_free(num3);
	TR_BigInt_free(num4);
	TR_BigInt_free(num5);
	TR_BigInt_free(num6);
	TR_Environment_free(env);
}
END_TEST

Suite* test_suite()
{
	Suite *suite;
	TCase *testCore,*testComparisons;
	
	suite = suite_create("BigInt");
	testCore = tcase_create("Operations");
	tcase_add_test(testCore,bigint_subtract);
	tcase_add_test(testCore,bigint_add);
	tcase_add_test(testCore,bigint_init);
	tcase_add_test(testCore,bigint_gcd);
	tcase_add_test(testCore,bigint_multiply);
	tcase_add_test(testCore,bigint_divide);

	testComparisons = tcase_create("Comparisons");
	tcase_add_test(testComparisons,bigint_compare);

	suite_add_tcase(suite,testComparisons);
	suite_add_tcase(suite,testCore);

	return suite;
}

