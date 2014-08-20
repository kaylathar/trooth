#include "BigInt.h"
#include <stdio.h>


int main()
{
	TR_BigInt* k = TR_BigInt_fromString("1234567809");

	printf("%s",TR_bigint_toString(k));

	TR_BigInt_free(k);

	return 0;
}
