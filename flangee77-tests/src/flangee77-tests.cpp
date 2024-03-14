
#include <TestLabs/TestSuite.h>
#include <CoreLabs/logging.h>

#include "./CoreLabs/bits.h"
#include "./CoreLabs/strings.h"
#include "./CoreLabs/Version.h"

#include "./DataLabs/compression.h"

#include "./MathLabs/utilities.h"

#include "./XiaoLabs/graphics.h"



int main()
{
    cl7::logging::StandardLogger::instance().clear_log_handlers();

    tl7::TestSuite::instance().run_tests();

    return 0;
}
