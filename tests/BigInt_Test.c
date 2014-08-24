#include "BigInt.h"
#include <stdio.h>


int main()
{

	TR_BigInt* k1 = TR_BigInt_fromString("1234567809");
	TR_BigInt* k2 = TR_BigInt_fromString("12");

	TR_BigInt* k3 = TR_BigInt_add(k1,k2);	

	printf("Result %s\n",TR_BigInt_toString(k3));
	printf("Result %d\n",TR_BigInt_compare(k1,k2));
	printf("Result %d\n",TR_BigInt_compare(k2,k3));
	printf("Result %d\n",TR_BigInt_compare(k1,k1));

	TR_BigInt_free(k1);
	TR_BigInt_free(k2);
	TR_BigInt_free(k3);
	return 0;
}
