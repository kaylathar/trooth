#include "BigInt.h"
#include <stdio.h>
#include <check.h>

START_TEST (bigint_init)
{

}
END_TEST

START_TEST (bigint_add)
{

}
END_TEST

START_TEST (bigint_subtract)
{

}
END_TEST


int main()
{

	TR_Environment *env = TR_Environment_alloc();
	TR_BigInt* k1 = TR_BigInt_fromString(env,"1234567809");
	TR_BigInt* k2 = TR_BigInt_fromString(env,"12");

	TR_BigInt* k3 = TR_BigInt_add(env,k1,k2);
	TR_BigInt* k4 = TR_BigInt_subtract(env,k1,k2);
	TR_BigInt* k5 = TR_BigInt_subtract(env,k2,k1);	

	printf("Result %s\n",TR_BigInt_toString(env,k3));
	printf("Result %d\n",TR_BigInt_compare(env,k1,k2));
	printf("Result %d\n",TR_BigInt_compare(env,k2,k3));
	printf("Result %d\n",TR_BigInt_compare(env,k1,k1));
	printf("Result %s\n",TR_BigInt_toString(env,k4));
	printf("Result %s\n",TR_BigInt_toString(env,k5));

	TR_BigInt_free(env,k1);
	TR_BigInt_free(env,k2);
	TR_BigInt_free(env,k3);
	TR_BigInt_free(env,k4);
	TR_BigInt_free(env,k5);
	return 0;
}
