
#include <TestLabs/TestSuite.h>
#include <CoreLabs/logging.h>

#include "./CoreLabs/bits.h"
#include "./CoreLabs/strings.h"
#include "./CoreLabs/Guid.h"
#include "./CoreLabs/Version.h"

#include "./DataLabs/Base64.h"
#include "./DataLabs/compression.h"
#include "./DataLabs/syntax.h"

#include "./MathLabs/functions.h"
#include "./MathLabs/Vector2.h"
#include "./MathLabs/Vector3.h"
#include "./MathLabs/Vector4.h"
#include "./MathLabs/Matrix2x2.h"
#include "./MathLabs/Matrix2x3.h"
#include "./MathLabs/Matrix3x3.h"
#include "./MathLabs/Matrix3x4.h"
#include "./MathLabs/Matrix4x4.h"
#include "./MathLabs/Quaternion.h"
#include "./MathLabs/Angle.h"

#include "./ProgLabs/ArgumentBag.h"

#include "./XiaoLabs/graphics.h"



int main()
{
    cl7::logging::StandardLogger::instance().clear_log_handlers();

    tl7::TestSuite::Settings settings;
    settings.max_check_fail_count = 0;
    tl7::TestSuite::instance().set_settings(settings);

    tl7::TestSuite::instance().run_tests();

    return 0;
}
