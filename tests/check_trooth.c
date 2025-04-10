#include <stdlib.h>
#include <check.h>

Suite* test_suite();

int main()
{
        int number_failed;
        SRunner *suiteRunner;

        suiteRunner = srunner_create(test_suite());

        srunner_run_all(suiteRunner,CK_VERBOSE);
        number_failed = srunner_ntests_failed(suiteRunner);
        return number_failed==0?EXIT_SUCCESS:EXIT_FAILURE;
}
