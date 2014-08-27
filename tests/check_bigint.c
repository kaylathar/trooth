#include "BigInt.h"
#include "util.h"
#include <stdlib.h>
#include <check.h>

#define LARGE_CHECK_NUMBER "1234123412341234123412341234123412341234123412341234123412341234123412341234123412341234123412341234012439082349082093489023840982340982340982390482903482093482093849028340928349082309473149057932048509238402834092834902834098230948209589043703485092834098234098230942903390573049753745732098349082309482934890234750934750928345098213409813405782309452703457092347850283450989230458023497590145987230485089234570237450982345"
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
	TR_BigInt *num1 = TR_BigInt_fromString(env,"12");
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

Suite* bigint_suite()
{
	Suite *suite;
	TCase *testCore;
	
	suite = suite_create("BigInt");
	testCore = tcase_create("Core");

	tcase_add_test(testCore,bigint_subtract);
	tcase_add_test(testCore,bigint_add);
	tcase_add_test(testCore,bigint_init);
	tcase_add_test(testCore,bigint_compare);

	suite_add_tcase(suite,testCore);

	return suite;
}

int main()
{
	int number_failed;
	Suite *suite;
	SRunner *suiteRunner;

	suite = bigint_suite();
	suiteRunner = srunner_create(suite);

	srunner_run_all(suiteRunner,CK_NORMAL);
	number_failed = srunner_ntests_failed(suiteRunner);
	return number_failed==0?EXIT_SUCCESS:EXIT_FAILURE;
}
