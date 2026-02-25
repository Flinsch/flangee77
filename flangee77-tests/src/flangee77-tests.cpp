
#include <TestLabs/TestSuite.h>

#include <CoreLabs/logging.h>



int main()
{
    cl7::logging::StandardLogger::instance().clear_log_handlers();

    tl7::TestSuite::Settings settings;
    settings.max_check_fail_count = 0;
    tl7::TestSuite::instance().set_settings(settings);

    tl7::TestSuite::instance().run_tests();

    return tl7::TestSuite::instance().get_stats().cases.fail_count > 0 ? 1 : 0;
}
