#include <stdlib.h>
#include <check.h>
#include "check_bigint.h"

int main()
{
        int number_failed;
        Suite *suite;
        SRunner *suiteRunner;

        suite = bigint_suite();
        suiteRunner = srunner_create(suite);
	srunner_add_suite(suiteRunner,fraction_suite());

        srunner_run_all(suiteRunner,CK_NORMAL);
        number_failed = srunner_ntests_failed(suiteRunner);
        return number_failed==0?EXIT_SUCCESS:EXIT_FAILURE;
}
