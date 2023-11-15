
#include <TestLabs/TestSuite.h>
#include <CoreLabs/logging.h>

#include "./CoreLabs/strings.h"



int main()
{
    cl7::logging::StandardLogger::instance().clear_log_handlers();

    tl7::TestSuite::instance().run_tests();

    return 0;
}
