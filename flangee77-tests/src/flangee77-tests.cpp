
#include <TestLabs/TestSuite.h>
#include <CoreLabs/logging.h>

#include "./CoreLabs/bits.h"
#include "./CoreLabs/strings.h"
#include "./CoreLabs/Base64.h"
#include "./CoreLabs/Guid.h"
#include "./CoreLabs/Version.h"

#include "./DataLabs/compression.h"

#include "./MathLabs/utilities.h"
#include "./MathLabs/Vector2.h"
#include "./MathLabs/Vector3.h"

#include "./XiaoLabs/graphics.h"



int main()
{
    cl7::logging::StandardLogger::instance().clear_log_handlers();

    tl7::TestSuite::Settings settings;
    settings.max_check_fail_count = 0;
    tl7::TestSuite::instance().settings = settings;

    tl7::TestSuite::instance().run_tests();

    return 0;
}
